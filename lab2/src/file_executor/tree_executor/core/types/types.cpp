#include "types.hpp"

namespace tree_executor
{

    void tree_executor::ExObj::RunMethod(const std::string &name, const std::vector<ExObjUPtr> &args)
    {
        auto it = methods.find(name);
        if (it != methods.end())
        {
            it->second(args);
        }
        else
        {
            throw std::runtime_error("Method not found: " + name);
        }
    }
    
} // namespace tree_executor
