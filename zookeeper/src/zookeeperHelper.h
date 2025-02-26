#pragma once
#include <memory>
#include <string_view>
#include <functional>
#include <exception>
#include <string>

/// @brief �ڵ�Ȩ��
enum class ZK_ACL
{
	Open, 		//ȫ���ţ�����Ȩ�޿���
	OnlyRead, 	//ֻ��
	CreatorOpen	//������ӵ������Ȩ��
};

/// @brief �ڵ�����
enum class ZK_NodeMode
{
	Persistent = 0,		//�־ýڵ�
	Ephemeral = 1,  //��ʱ�ڵ�
	Persistent_Sequential = 2,  //�־�˳��ڵ�
	Ephemeral_Sequential= 3,	//��ʱ˳��ڵ�
	Container = 4,				//�����ڵ�
	PersistentWithTTL = 5,
	Persistent_Sequential_WithTTL = 6
};

/// @brief Session�¼�����
enum class ZK_SessionEvent
{
	Connecting,	//���ڽ�������
	Connected,	//���ӳɹ�
	Expired,	//�Ự����
				//	 ���ں���Ե���recreate_session()�ؽ�����zookeeper�ĻỰ
				//	 ע�������Ӻ�����µ���race_master()����master��
				//	 �򵥵��������session���ں��˳���ǰ���̣�ͨ�����������ķ�ʽ������master
				//
				//	 �ر�ע�⣨session���ڵ�ǰ���������Ѿ���������
				//	 ������ӱ����𣬲��ᴥ��on_zookeeper_session_expired()�¼���
				//	 ������ѡmasterʱ����������Ҫ�Լ�ά����������µĳ�ʱ��
				//	 �Ա�����ʱ�ڵ㱻ɾ������Ȼ����Ϊmaster״̬��
	Unknown
};

typedef struct clientid_t clientid_t;
typedef struct _zhandle zhandle_t;

class ZookeeperClient
{
 public:
	explicit ZookeeperClient(std::string_view host) noexcept;
	~ZookeeperClient() noexcept =default;

	void connect(time_t timeout,const std::function<void (ZK_SessionEvent,std::string_view)>& callback);

	void close();

	/// @brief ��ȡ���ӳɹ���ip:port
	/// @return �������ӳɹ���ip:port���������ӷ��ؿ��ַ���
	std::string getConnectedHost() const noexcept;

	bool isConnected() const noexcept;

	/// @brief ������Ϊmaster
	/// @param zk_path ���ھ�����path������Ϊ�գ��Ҹ�path�����Ѿ�����
	/// @param data �洢��zk_path�����ݣ�������ʶ�浱ǰ��master����˿��Կ���ʹ��IP�������б�ʶ��ֵ
	/// @return ��������ɹ�����true�����򷵻�false
	bool raceMaster(std::string_view zk_path,std::string_view data);

	/// @brief ����һ���ڵ㣬Ҫ�󸸽ڵ�����Ѿ���������Ȩ��
	/// @param path ���ڵ�·��
	/// @param nodeName �ڵ���
	/// @param data �ڵ�����
	/// @param nodeType �ڵ�����
	/// @param mode �ڵ�Ȩ��
	void createNode(std::string_view path,std::string_view nodeName,std::string_view data,ZK_NodeMode mode,ZK_ACL acl);

	/// @brief ɾ���ڵ㣬��У��汾�ţ�����һ����ɾ��ʧ��
	/// @param nodePath �ڵ�·��
	/// @param version �ڵ�汾��-1��У��
	void deleteNode(std::string_view nodePath,int version);
	/// @brief ɾ���ڵ㣬��У��汾
	void deleteNode(std::string_view nodePath);


	/// @brief ��ȡ·���������ӽڵ�
	/// @param path �ڵ�·��
	/// @return �ӽڵ�vector
	std::vector<std::string> getAllChildren(std::string_view path) ;

	/// @brief ��ȡ�ڵ�����
	/// @param path �ڵ�·��
	/// @param maxSize �洢�ڵ����ݵ�����С
	/// @param keep_watch �Ƿ񱣳�watch��path
	/// @return �ڵ�����
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
	/// @brief �¼������Լ��ַ�
	/// @param zh zk���
	/// @param type �¼�����
	/// @param state ״̬
	/// @param path �ڵ�·��
	/// @param context ������
	static void zk_watcher(zhandle_t *zh, int type, int state, const char *path, void *context);

	std::string _zkHost;
	std::unique_ptr<clientid_t,std::function<void (clientid_t*)>> _zkClient;
	std::unique_ptr<zhandle_t,std::function<void (zhandle_t*)>> _handle;
	std::function<void (ZK_SessionEvent,std::string_view)> _sessionCallback;

};