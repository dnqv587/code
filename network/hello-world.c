/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <ctype.h>

static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 9995;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
    struct sockaddr *, int socklen, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

int
main(int argc, char **argv)
{
	struct event_base *base;   //�ػ�
	struct evconnlistener *listener; //���Ӽ�����
	struct event *signal_event;  //�ź��¼�

	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	//�����ػ�---�൱��epoll������(epoll_create)
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	//�������Ӽ�����--socket-bind-listen-accept
	//listener_cb: �ص�����
	//LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE: ���ö˿ڸ���, �����Ӽ������ͷŵ�ʱ��ر��׽���(�����ļ�������)
	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)&sin,
	    sizeof(sin));

	if (!listener) {
		fprintf(stderr, "Could not create a listener!\n");
		return 1;
	}

	//����SIGINT�źŵ��¼��ص�
	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0) {
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

	//����ȴ��¼�ѭ��---�൱��while(1)
	event_base_dispatch(base);

	//�ͷ���Դ
	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}


//listener: ���Ӽ�����
//fd: ͨ���ļ�������
//sa��socklen: �ͻ���IP��ַ��Ϣ
//user_data: ����
static void
listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
	struct event_base *base = user_data;
	struct bufferevent *bev;

	//����bufferevent������
	//BEV_OPT_CLOSE_ON_FREE: bufferevent�ͷŵ�ʱ���Զ��ر�ͨ���ļ�������
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);//�˳�ѭ��,�������
		return;
	}
	//���ûص�����: ���ص�, д�ص����¼��ص�
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	//ʹbufferevent������Ч
	bufferevent_enable(bev, EV_WRITE);
	bufferevent_enable(bev, EV_READ);
	
	//ʹbufferevent������Ч
	//bufferevent_disable(bev, EV_READ);

	//bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void
conn_readcb(struct bufferevent *bev, void *user_data)
{
	//size_t bufferevent_read(struct bufferevent *bufev, void *data, size_t size);
	char buf[1024];
	memset(buf, 0x00, sizeof(buf));
	//��bufferevent������
	int n = bufferevent_read(bev, buf, sizeof(buf));
	printf("n=[%d],buf==[%s]\n", n, buf);
	
	int i=0;
	for(i=0; i<n; i++)
	{
		buf[i] = toupper(buf[i]);
	}
	//��bufferevent��д������д����
	//int bufferevent_write(struct bufferevent *bufev, const void *data, size_t size);
	bufferevent_write(bev, buf, n);//дbufferevent�������ᴥ��д�¼��ص�
}

static void
conn_writecb(struct bufferevent *bev, void *user_data)
{
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) {
		printf("flushed answer\n");
		//bufferevent_free(bev);
	}
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
		    strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bev);
}

static void
signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}
