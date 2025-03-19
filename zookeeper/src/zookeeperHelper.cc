#include "zookeeperHelper.h"
#include <zookeeper.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

namespace detail
{
static inline constexpr ACL_vector getZookeeperMode(ZK_ACL mode)
{
	if (ZK_ACL::Open == mode)
	{
		return ZOO_OPEN_ACL_UNSAFE;
	}
	else if(ZK_ACL::OnlyRead == mode)
	{
		return ZOO_READ_ACL_UNSAFE;
	}
	else if(ZK_ACL::CreatorOpen == mode)
	{
		return ZOO_CREATOR_ALL_ACL;
	}
    return ZOO_OPEN_ACL_UNSAFE;
}

static inline std::string getErrorInfo(int errcode)
{
	return " zookeeper error," + std::string(zerror(errcode)) + " (" + std::to_string(errcode) + ")";
}

static inline ZK_SessionEvent ZKStatToSessionEvent(const int type)
{
	if(ZOO_EXPIRED_SESSION_STATE == type)
	{
		return ZK_SessionEvent::Expired;
	}
	else if(ZOO_CONNECTED_STATE == type)
	{
		return ZK_SessionEvent::Connected;
	}
	else if(ZOO_CONNECTING_STATE == type)
	{
		return ZK_SessionEvent::Connecting;
	}
	else
	{
		return ZK_SessionEvent::Unknown;
	}
}

}

ZookeeperClient::ZookeeperClient(std::string_view host) noexcept
:_zkHost(host),
_zkClient(nullptr),
_handle(nullptr,[this](zhandle_t* handle){
  this->close();
}),
 _isConnected(false),
 _sessionCallback([](ZK_SessionEvent event){
 })
{}

void ZookeeperClient::connect(time_t timeout,const std::function<void (ZK_SessionEvent)>& callback)
{
	_handle.reset(zookeeper_init(_zkHost.c_str(),ZookeeperClient::zk_watcher,timeout * 1000l ,_zkClient.get(),this,0));
	// 当连接不上时，会报如下错误（默认输出到stderr，可通过zoo_set_log_stream(FILE*)输出到文件）：
	// zk retcode=-4, errno=111(Connection refused): server refused to accept the client
	if(nullptr == _handle)
	{
		throw zkError("connect node://" + _zkHost + " failed,"+ strerror(errno));
	}
	_sessionCallback = callback;
}

void ZookeeperClient::close()
{
	if(_handle)
	{
		int errcode = zookeeper_close(_handle.get());
		if (errcode != ZOK)
		{
			throw zkError("zookeeper close node://" + _zkHost + "failed:" + zerror(errcode) + "(" +std::to_string(errcode) + ")");
		}
		_handle.reset();
	}
}

std::string ZookeeperClient::getConnectedHost() const noexcept
{
	struct sockaddr_in6 addr_in6{};
	socklen_t addr_len = sizeof(addr_in6);
	if (nullptr == zookeeper_get_connected_host(_handle.get(), reinterpret_cast<struct sockaddr*>(&addr_in6), &addr_len))
	{
		return {};
	}

	const struct sockaddr* addr = reinterpret_cast<struct sockaddr*>(&addr_in6);
	if (AF_INET == addr->sa_family)
	{
		const auto* addr_in = reinterpret_cast<const struct sockaddr_in*>(addr);
		std::string host;
		host.resize(32);
		::inet_ntop(AF_INET,&addr_in->sin_addr,host.data(),host.size());
		host.append(std::to_string(ntohs(addr_in->sin_port)));
		return host;
	}
	else if (AF_INET6 == addr->sa_family)
	{
		const auto* addr_in = reinterpret_cast<const struct sockaddr_in6*>(addr);
		std::string host;
		host.resize(128);
		::inet_ntop(AF_INET6,&addr_in->sin6_addr,host.data(),host.size());
		host.append(std::to_string(ntohs(addr_in->sin6_port)));
		return host;
	}
	return {};
}
bool ZookeeperClient::isConnected() const noexcept
{
	return ZOO_CONNECTED_STATE == zoo_state(_handle.get());
}

bool ZookeeperClient::raceMaster(std::string_view zk_path, std::string_view data)
{
	const int errcode = zoo_create(_handle.get(), zk_path.data(), data.data(), static_cast<int>(data.size()), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, nullptr, 0);
	if(ZOK == errcode)
	{
		return true;
	}
	if(ZNODEEXISTS == errcode)
	{
		return false;
	}
	else
	{
		throw zkError("raceMaster error," + std::string(zerror(errcode)) + "(" + std::to_string(errcode) + ")");
	}
}
void ZookeeperClient::createNode(std::string_view path,
	std::string_view node,
	std::string_view data,
	ZK_NodeMode mode,
	ZK_ACL acl)
{
	std::string nodePath(path);
	nodePath.erase(0,nodePath.find_first_not_of(' '));
	nodePath.erase(nodePath.find_last_not_of(' ') +1);
	if(nodePath.back() == '/')
	{
		nodePath.append(node);
	}
	else
	{
		nodePath.push_back('/');
		nodePath.append(node);
	}
	const auto aclVector = detail::getZookeeperMode(acl);
	const int errcode = zoo_create(_handle.get(), nodePath.c_str(), data.data(), static_cast<int>(data.size()), &aclVector,static_cast<int>(mode),nullptr, 0);
	if (errcode != ZOK)
	{
		throw zkError("create path://" + nodePath + detail::getErrorInfo(errcode));
	}
}
void ZookeeperClient::deleteNode(std::string_view nodePath)
{
	deleteNode(nodePath,-1);
}
void ZookeeperClient::deleteNode(std::string_view nodePath, int version)
{
	const int errcode = zoo_delete(_handle.get(), nodePath.data(), version);
	if(ZOK != errcode)
	{
		throw zkError("delete path://" + std::string(nodePath) + detail::getErrorInfo(errcode));
	}
}
std::vector<std::string> ZookeeperClient::getAllChildren(std::string_view path) const
{
	struct String_vector children{};
	std::vector<std::string> childrenVec;
	const int errcode = zoo_get_children(_handle.get(),path.data(),0,&children);
	if(ZOK != errcode)
	{
		throw zkError("get path://" + std::string(path) + "children failed,"+ detail::getErrorInfo(errcode));
	}
	childrenVec.reserve(children.count);
	for(int i =0;i<children.count;++i)
	{
		childrenVec.emplace_back(children.data[i]);
	}
	deallocate_String_vector(&children);
	return childrenVec;
}
std::string ZookeeperClient::getData(std::string_view path, int maxSize, bool keep_watch) const
{
	std::string data;
	data.resize(maxSize);
	struct Stat stat{};
	const int errcode = zoo_get(_handle.get(),path.data(),keep_watch?1:0,data.data(),&maxSize,&stat);
	if(ZOK != errcode)
	{
		throw zkError("get path://" + std::string(path) + "data failed,"+ detail::getErrorInfo(errcode));
	}
	if(maxSize>0)
	{
		data.resize(maxSize);
	}
	return data;
}

void ZookeeperClient::zk_watcher(zhandle_t* zh, int type, int state, const char* path, void* context)
{
	auto* self = static_cast<ZookeeperClient*>(context);
	if (type != ZOO_SESSION_EVENT)
	{

	}
	else
	{
		// 0: 初始状态（也就是没有状态的状态）
		// 1: ZOO_CONNECTING_STATE
		// 2: ZOO_ASSOCIATING_STATE
		// 3: ZOO_CONNECTED_STATE
		// -112: ZOO_EXPIRED_SESSION_STATE
		// -113: ZOO_AUTH_FAILED_STATE
		// 999: NOTCONNECTED_STATE_DEF

		// 状态间转换：
		//    ZOO_CONNECTING_STATE
		// -> ZOO_ASSOCIATING_STATE
		// -> ZOO_CONNECTED_STATE/ZOO_EXPIRED_SESSION_STATE
		// -> ZOO_AUTH_FAILED_STATE

        if(ZOO_CONNECTED_STATE == state)
        {
            {
                std::lock_guard lock(self->_mutex);
                self->_isConnected = true;
            }
            self->_cond.notify_all();
        }

		std::invoke(self->_sessionCallback,detail::ZKStatToSessionEvent(state));
	}
}

void ZookeeperClient::waitConnected() noexcept
{
    std::unique_lock lock(_mutex);
    if(!_isConnected)
    {
        _cond.wait(lock,[this]{
            return _isConnected;
        });
    }
}



