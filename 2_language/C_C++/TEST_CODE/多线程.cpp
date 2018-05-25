//1async(fun)//老板分配一个任务不管不问；async(std::launch::async,fun)老板分配一个任务，指示立即去做；async(std::launch::deffered,fun)老板分配一个任务，等收到信息后(get())再做。get()老板要一个结果，wait(),老板等待职员做完，但并没有要结果；wait_for老板限时间周期等待职员做完，且职员会反馈一个工作状态，wait_until老板限定特定时间点等待做完，且职员会返回一个工作状态。
//2get()函数只准被调用一次，如果要调用多次，需要使用shared_future
//3传递给子线程的变量最好是值传递，因为在非launch::async的情况下，子线程的开始执行可能比较晚，所以在主线程传给子线程的局部变量可能 在子线程未执行前就因为主线程运行完而被销毁或是局部被其它语句改变。
#include<iostream>
//标准C++多线程应包含头文件：
#include<thread>
#include<future>
#include<chrono>
#include<cstdlib>
#include<string>
using namespace std;
void fun1(void)
{
	cout<<"fun1 called "<<endl;
}
int fun2(int a)
{
	cout<<"fun2 called with:"<<a<<endl;
	std::this_thread::sleep_for(chrono::milliseconds(1000));
	//std::this_thread::yield();//转交CPU
	a=a*a;
	return a;
};
////////////////////////////////////////////////////////////////////////////////
void fun3(std::promise<string> &p,string &s,int flag)
{
	cout<<"fun3 called with:"<<flag<<endl;
	p.set_value("just do it");
	s=string("just do what you want");
}
int main()
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"/////////////////////////////////////////////////////////////////"<<endl;
    cout<<"=>多线程high-level接口"<<endl;
	{
		int in=100;
		//创建线程格式：
		future<void> result1=async(fun1);//线程什么时候被执行和什么时候结束由系统决定，如果后面没有显式地使用result1.get(),result1.wait(),result1.wait_for(),result1.wait_until();那么线程可能在主线程执行完都一直不被执行，这样临时变量result1就失去了意义，而它达要接收async的返回值，这就产生了错误。
		future<void> result2=async(std::launch::async,fun1);//使用std::launch::async可以使线程立即被执行，而且即使不使用get,wait等函数，这个主线程（指创建该线程的线程）也会等到子线程结束后才会结束。
		future<void> result3=async(std::launch::deferred,fun1);//等到显式地使用get()函数后才开始执行。

		//当被调用的函数有参数时，async的函数实参后面仅跟被调用函数的实参。
		future<int> result11=async(fun2,in);
		future<int> result22=async(std::launch::async,fun2,in);
		future<int> result33=async(std::launch::deferred,fun2,std::ref(in));//虽说std::ref()传递的是值引用，但值并不有改变
		result33.wait();cout<<"in:"<<in<<endl;//等待线程运行完

		cout<<"----------------------------------------------"<<endl;
		//=>使用get()和wait()可以使主线程立即起用子线程，并等待其运行完。
		result1.get();//功能同上，促使result1对应的线程运行完
		//result3.wait_for(std::chrono::seconds(5));//在这里等待子线程5秒后继续运行
		//result3.wait_for();可以无实参，只是返回一个线程的状态
		//result11.wait_until(std::chrono::system_clock::now()+std::chrono::seconds(5));//等到特定的时间继续运行
		cout<<result22.get()<<endl;//使用函数的返回值

		cout<<"----------------------------------------------"<<endl;//
		future<void> result4=async([](){cout<<"using lambda function"<<endl;});
		//=>wait_for和wait_until函数会返回一个funture_status， 有三种情况：
		//	Both wait_for() and wait_until() return one of the following:
		// std::future_status::deferred if async() deferred the operation and no calls to wait()
		//or get() have yet forced it to start (both function return immediately in this case)
		// std::future_status::timeout if the operation was started asynchronously but hasn’t finished
		//yet (if the waiting expired due to the passed timeout)
		// std::future_status::ready if the operation has finished
		std::future_status fs=result4.wait_for(std::chrono::seconds(0));
		if(fs==std::future_status::deferred) cout<<"future_status::deferred"<<endl;
		if(fs==std::future_status::ready) cout<<"future_status::ready"<<endl;
		if(fs==std::future_status::timeout) cout<<"future_status::timeout"<<endl;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"/////////////////////////////////////////////////////////////////"<<endl;
    cout<<"=>下面是low-level多线程接口"<<endl;
//
	{
		std::thread t1(fun1);//创建一个线程。线程只要被创建就会立即执行，相当于async的launch::async
		std::thread t2(fun2,7);//创建一个带参数的线程
		thread::id t1id=t1.get_id();cout<<"thread t1's id:"<<t1id<<endl;
		cout<<"主线程id:"<<this_thread::get_id<<endl;
		if(this_thread::get_id()!=t1id) cout<<"线程id可以用来比较，除此不能有其它操作"<<endl;
//1线程分为两种，一种是在主线程运行结束前，副线程就结束，有两种方式，在主线程结束前对所有的副线程使用join()函数等待其完成，或直接使用notify_all_at_thread_exit();使主线程总是等待副线程运行完才结束。另一种是用detach()函数使线程在后台运行，主线程结束后它仍然可以运行，为避免变量在主线程结束后被销毁的问题，有两种方式，所有传递给副线程的参数必须是一个copy值，或者主线程使用quick_exit()，这时所有static变量和全局变量不会被销毁而留作副线程使用。

		t1.join();//等待该线程运行完
		t2.detach();//使线程后台运行

		//notify_all_at_thread_exit();//这个函数还不会使用
//		quick_exit();//这个函数在cstdlib.h中声明，该函数还不会使用
        cout<<"----------------------------------------"<<endl;
		promise<string> result;//low-level多线程接口参数传递的几种方式
		string s;
		int flag=111;
		thread t3(fun3,std::ref(result),ref(s),flag);//一定要使用ref函数，为thread模板指明是引用类型
		t3.join();
		cout<<result.get_future().get()<<endl;
		cout<<s<<endl;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"/////////////////////////////////////////////////////////////////"<<endl;
    cout<<"=>使用packaged_task<>可以预先建立一个线程后面运行"<<endl;
	{
//		double compute (int x, int y);
//		std::packaged_task<double(int,int)> task(compute); // create a task
//		std::future<double> f = task.get_future(); // get its future
//		...
//		task(7,5); // start the task (typically in a separate thread)
//		...
//		double res = f.get(); // wait for its end and process result/exception
	}
	return 0;
}



