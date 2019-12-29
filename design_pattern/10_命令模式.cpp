#include <iostream>
#include <vector>

using namespace std;

class Command
{
public:
  virtual void execute() = 0;
};

class ConcreteCommand1 : public Command
{
public:
  virtual void execute()
  {
    cout << "具体指令1" << endl;
  }
};

class ConcreteCommand2 : public Command
{
public:
  virtual void execute()
  {
    cout << "具体指令2" << endl;
  }
};

class MacroCommand : public Command
{
public:
  void add_command(Command* command)
  {
    commands.push_back(command);
  }
  virtual void execute()
  {
    for(vector<Command*>::iterator it = commands.begin(); it != commands.end(); it++)
    {
      (*it)->execute();
    }
  }
private:
  vector<Command*> commands;
};

int main()
{
  ConcreteCommand1 command1;
  ConcreteCommand2 command2;

  MacroCommand* mac_cmd = new MacroCommand();
  mac_cmd->add_command(&command1);
  mac_cmd->add_command(&command2);
  mac_cmd->execute();
  return 0;
}
