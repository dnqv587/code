#pragma once
#include "../base/noncopyable.h"
#include <pthread.h>
#include <assert.h>
/*
封装mutex
*/

//clang的线程安全注解--需加入编译选项 -Wthread-safety
// https://clang.llvm.org/docs/ThreadSafetyAnalysis.html

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

//修饰类的宏
/*
capability 是 TSA 中的⼀个概念，⽤来为资源的访问提供相应的保护。这⾥的资源可以是数据成员，也可以是访问某些潜在资源的函数/
⽅法。capability 通常表现为⼀个带有能够获得或释放某些资源的⽅法的对象，最常见的就是 mutex 互斥锁。换⾔之，⼀个 mutex 对象就
是⼀个 capability
*/

//CAPABILITY 表明某个类对象可以当作 capability 使⽤，其中 x 的类型是 string，能够在错误信息当中指出对应的 capability 的名称
#define CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

//SCOPED_CAPABILITY ⽤于修饰基于 RAII 实现的 capability
#define SCOPED_CAPABILITY \
  THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)
//end

//GUARD_BY ⽤于修饰对象，表明该对象需要受到 capability 的保护
#define GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))

//PT_GUARDED_BY⽤于修饰指针类型变量，在更改指针变量所指向的内容前需要加锁，否则发出警告
#define PT_GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(x))

//ACQUIRED_BEFORE 和 ACQUIRED_AFTER 主要⽤于修饰 capability 的获取顺序，⽤于避免死锁
#define ACQUIRED_BEFORE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ACQUIRED_AFTER(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

 //REQUIRES 声明调⽤线程必须拥有对指定的 capability 具有独占访问权。可以指定多个 capabilities。函数/⽅法在访问资源时，必须先上锁，再调⽤函数，然后再解锁
#define REQUIRES(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

//REQUIRES_SHARED 功能与 REQUIRES 相同，但是可以共享访问
#define REQUIRES_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

//CQUIRE 表⽰⼀个函数/⽅法需要持有⼀个 capability，但并不释放这个 capability。调⽤者在调⽤被 ACQUIRE 修饰的函数/⽅法时，要确保没有持有任何 capability，进入函数时才会持有capability
#define ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

 //ACQUIRE_SHARED 与 ACQUIRE 的功能是类似的，但持有的是共享的 capability
#define ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

//RELEASE 和 RELEASE_SHARED 与 ACQUIRE 和 ACQUIRE_SHARED 正相反，它们表⽰调⽤⽅在调⽤该函数/⽅法时需要先持有锁，⽽当函数执⾏结束后会释放锁
#define RELEASE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define RELEASE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define TRY_ACQUIRE(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define TRY_ACQUIRE_SHARED(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

//EXCLUDES ⽤于显式声明函数/⽅法不应该持有某个特定的 capability。由于 mutex 的实现通常是不可重⼊的，因此 EXCLUDES 通常被⽤来预防死锁
#define EXCLUDES(...) \
  THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

//ASSERT_* 表⽰在运⾏时检测调⽤线程是否持有 capability
#define ASSERT_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define ASSERT_SHARED_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

//RETURN_CAPABILITY 通常⽤于修饰那些被当作 capability getter 的函数，这些函数会返回 capability 的引⽤或指针
#define RETURN_CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

//NO_THREAD_SAFETY_ANALYSIS 表⽰关闭某个函数/⽅法的 TSA 检测，通常只⽤于两种情况：1，该函数/⽅法可以被做成⾮线程安全；2、函数/⽅法太过复杂
#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

// End of thread safety annotations }

//pthread错误检查
#define CHECK_PTHREAD_RETURN_VALUE
#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
#ifdef __cplusplus
extern "C" {
#endif 
		extern void __assert_perror_fail(int errnum,
		const char* file,
		unsigned int line,
		const char* function)
		noexcept __attribute__((__noreturn__));
#ifdef __cplusplus
}
#endif 
#endif
//__builtin_expect 提供给程序员使用的，目的是将“分支转移”的信息提供给编译器，这样编译器能够对代码进行优化，以减小指令跳转带来的性能降低
#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})
//(void)errnum  做用仅仅就是以显眼的方式让编译器不要给出参数未被使用的警告
#endif // CHECK_PTHREAD_RETURN_VALUE

class MutexLock :private noncopyable
{
public:
	MutexLock() 
	{
		pthread_mutex_init(&m_mutex, NULL);
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	void lock()//只可MutexLockGuard调用，禁止用户调用
	{
		pthread_mutex_lock(&m_mutex);
	}
	void unlock()////只可MutexLockGuard调用，禁止用户调用
	{
		pthread_mutex_unlock(&m_mutex);
	}
	pthread_mutex_t* getMutexLockPtr()
	{
		return &m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

/*
RAII技法
*/
class MutexLockGuard:private noncopyable
{
public:
	MutexLockGuard(MutexLock& mutex):m_MutexLock(mutex)
	{
		m_MutexLock.lock();
	}
	~MutexLockGuard()
	{
		m_MutexLock.unlock();
	}
private:
	MutexLock& m_MutexLock;
};

#define MutexLockGuard(x) static_assert(false,"missing MutexLockGuard var name")//防止未定义变量名，导致产生临时对象