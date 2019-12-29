#include <iostream>

//装饰模式(Decorator Pattern)：动态地给一个对象增加一些额外的职责

using namespace std;

class Call
{
public:
    virtual void callout()
    {
        cout << "发起呼叫" << endl;
    }
};

class VideoCall : public Call
{
public:
    virtual void callout()
    {
        cout << "发起视频呼叫" << endl;
    }
};

class AudioCall : public Call
{
public:
    virtual void callout()
    {
        cout << "发起音频呼叫" << endl;
    }
};

class Decorate : public Call
{
public:
    Decorate(Call* call)
    {
        this->call = call;
    }
protected:
    Call* call;
};

class PreproDecorate : public Decorate
{
public:
    PreproDecorate(Call* call) : Decorate(call) {}
    virtual void callout()
    {
        preprocall();
        call->callout();
    }
private:
    void preprocall()
    {
        cout << "呼叫预处理" << endl;
    }
};

class LogDecorate : public Decorate
{
public:
    LogDecorate(Call* call) : Decorate(call) {}
    virtual void callout()
    {
        pre_logging();
        call->callout();
        post_logging();
    }
private:
    void pre_logging()
    {
        cout << "进入日志打印"  << endl;
    }
    void post_logging()
    {
        cout << "退出日志打印" << endl;
    }
};

int main()
{
    VideoCall* vid_call = new VideoCall();
    PreproDecorate* pre_deco = new PreproDecorate(vid_call);
    pre_deco->callout();

    cout << endl;

    //下面几行代码，装饰类套装饰类，可以实现多个装饰
    AudioCall* aud_call = new AudioCall();
    PreproDecorate* aud_pre_deco = new PreproDecorate(aud_call);
    LogDecorate* log_deco = new LogDecorate(aud_pre_deco);
    log_deco->callout();
    /*
    进入日志打印
    呼叫预处理
    发起音频呼叫
    退出日志打印
    */
    return 0;
}
