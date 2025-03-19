#pragma once
#include <memory>
#include <string_view>
#include <functional>
#include <exception>
#include <string>
#include <mutex>
#include <condition_variable>

/// @brief 节点权限
enum class ZK_ACL
{
	Open, 		//全开放，不做权限控制
	OnlyRead, 	//只读
	CreatorOpen	//创建者拥有所有权限
};

/// @brief 节点类型
enum class ZK_NodeMode
{
	Persistent = 0,		//持久节点
	Ephemeral = 1,  //临时节点
	Persistent_Sequential = 2,  //持久顺序节点
	Ephemeral_Sequential= 3,	//临时顺序节点
	Container = 4,				//容器节点
	PersistentWithTTL = 5,
	Persistent_Sequential_WithTTL = 6
};

/// @brief Session事件类型
enum class ZK_SessionEvent
{
	Connecting,	//正在建立连接
	Connected,	//连接成功
	Expired,	//会话过期
				//	 过期后可以调用recreate_session()重建立与zookeeper的会话
				//	 注意重连接后得重新调用race_master()竞争master，
				//	 简单点的做法是session过期后退出当前进程，通过重新启动的方式来竞争master
				//
				//	 特别注意（session过期的前提是连接已经建立）：
				//	 如果连接被挂起，不会触发on_zookeeper_session_expired()事件，
				//	 当用于选master时，调用者需要自己维护这种情况下的超时，
				//	 以避免临时节点被删除后仍然保持为master状态。
	Unknown
};

struct clientid_t;
typedef struct _zhandle zhandle_t;

class ZookeeperClient
{
 public:
	explicit ZookeeperClient(std::string_view host) noexcept;
	~ZookeeperClient() noexcept =default;

	void connect(time_t timeout,const std::function<void (ZK_SessionEvent)>& callback);

	void close();

	/// @brief 获取连接成功的ip:port
	/// @return 返回连接成功的ip:port，若无连接返回空字符串
	std::string getConnectedHost() const noexcept;

	bool isConnected() const noexcept;

    void waitConnected() noexcept;

	/// @brief 竞争成为master
	/// @param zk_path 用于竞争的path，不能为空，且父path必须已经存在
	/// @param data 存储到zk_path的数据，可用于识辨当前的master，因此可以考虑使用IP或其它有标识的值
	/// @return 如果竞争成功返回true，否则返回false
	bool raceMaster(std::string_view zk_path,std::string_view data);

	/// @brief 创建一个节点，要求父节点必须已经存在且有权限
	/// @param path 父节点路径
	/// @param nodeName 节点名
	/// @param data 节点数据
	/// @param nodeType 节点类型
	/// @param mode 节点权限
	void createNode(std::string_view path,std::string_view nodeName,std::string_view data,ZK_NodeMode mode,ZK_ACL acl);

	/// @brief 删除节点，并校验版本号，若不一致则删除失败
	/// @param nodePath 节点路径
	/// @param version 节点版本，-1则不校验
	void deleteNode(std::string_view nodePath,int version);
	/// @brief 删除节点，不校验版本
	void deleteNode(std::string_view nodePath);


	/// @brief 获取路径在所有子节点
	/// @param path 节点路径
	/// @return 子节点vector
	std::vector<std::string> getAllChildren(std::string_view path) const;

	/// @brief 获取节点数据
	/// @param path 节点路径
	/// @param maxSize 存储节点数据的最大大小
	/// @param keep_watch 是否保持watch该path
	/// @return 节点数据
	std::string getData(std::string_view path,int maxSize=4096, bool keep_watch = false) const;



 class zkError:public std::exception
 {
  public:
	 zkError()=default;
	 explicit zkError(std::string_view info):_errorInfo(info){}
	 ~zkError() override = default;

	 inline const char* what() const noexcept override
	 {
		 return _errorInfo.c_str();
	 }
  private:
	 std::string _errorInfo;
 };
 private:
	/// @brief 事件监听以及分发
	/// @param zh zk句柄
	/// @param type 事件类型
	/// @param state 状态
	/// @param path 节点路径
	/// @param context 上下文
	static void zk_watcher(zhandle_t *zh, int type, int state, const char *path, void *context);

	std::string _zkHost;
	std::unique_ptr<clientid_t,std::function<void (clientid_t*)>> _zkClient;
	std::unique_ptr<zhandle_t,std::function<void (zhandle_t*)>> _handle;
    bool _isConnected;
	std::function<void (ZK_SessionEvent)> _sessionCallback;
    std::mutex _mutex;
    std::condition_variable _cond;
};