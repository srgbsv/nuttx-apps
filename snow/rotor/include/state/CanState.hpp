#pragma once

namespace Snow {

    class CanState
    {
    private:
        bool _inited = false;
        
    public:
        void canInit(bool init);
        void canRun(bool run);

        CanState(/* args */);
        ~CanState();
    };

    CanState::CanState(/* args */)
    {
        
    }

    CanState::~CanState()
    {
    }
    
}