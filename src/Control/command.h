#ifndef Command_h
#define Command_h

#include "Arduino.h"
#include "Movement.h"
#include <functional>
#include <vector>
class Commands
{
    std::vector<std::function<void(int,int)>>  cmds;
    std::vector<int[2]>  pars;
  public:
    
    void add(std::function<void(int,int)> bnd, int par1, int par2)
    {
      cmds.push_back( bnd);
      pars.push_back({par1,par2});
    }

    void run()
    {
      for(uint32_t i =0; i<cmds.size(); ++i)
      {
        cmds[i](pars[i][0], pars[i][1]);
      }
      cmds.clear();
      pars.clear();
    }
};

#endif
