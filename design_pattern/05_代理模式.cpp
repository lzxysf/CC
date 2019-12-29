#include <iostream>

//为其他对象提供一种代理（Proxy）以控制对这个对象的访问

using namespace std;

/*网站访问代理，访问网站前先判断网站是否合法*/

class Visit
{
public:
    virtual void create_socket(string protocol) = 0;
    virtual void send(string url) = 0;
};

class VisitHttp : public Visit
{
public:
    virtual void create_socket(string protocol)
    {
        cout << protocol << "网络建立完毕" << endl;
    }
    virtual void send(string url)
    {
        cout << url << " 发送成功" << endl;
    }
};

//绿色上网代理
class GreenHttpProxy : public Visit
{
public:
    GreenHttpProxy(VisitHttp* vh)
    {
        this->vh = vh;
    }
    virtual void create_socket(string protocol)
    {
        vh->create_socket(protocol);
    }
    virtual void send(string url)
    {
        if(is_fair(url))
        {
            visit_count++;
            vh->send(url);
        }
        else
        {
            cout << "网址非法" << endl;
        }
    }
    void get_visit_count()
    {
        cout << "访问次数:" << visit_count << endl;
    }
private:
    int visit_count = 0;
    VisitHttp* vh;
    bool is_fair(string url)
    {
        if(url.compare("www.1024.com")==0)
        {
            return false;
        }
        return true;
    }
};

int main()
{
  GreenHttpProxy* proxy = new GreenHttpProxy(new VisitHttp());
  proxy->create_socket("http");
  proxy->send("www.baidu.com");
  proxy->send("www.google.com");
  proxy->send("www.1024.com");
  proxy->get_visit_count();
  return 0;
}
