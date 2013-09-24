#include "game/ai/randommodule.h"

#include <cstdlib>

namespace ai {

RandomModule::RandomModule() : total_probability_(0.0) {}

RandomModule::~RandomModule() {}

void RandomModule::Start() {
    for (const auto& it : childs_)
        it.first->Start();
}

AIModule::Status RandomModule::Update(double dt, AIData* data) {
    // Choose a random number in [0, total_probability_[
    double choice = total_probability_ * (static_cast<double>(std::rand()) / RAND_MAX);

    for (const auto& it : childs_) {
        // If the 'random range' of the given 
        if(choice < it.second)
            return it.first->Update(dt, data);
        choice -= it.second;
    }

    //We don't have any child or we couldn't select a child module within our probability distribution...
    //the "couldn't select" part normally means the probability distribution is wrong...
    return DONE;
}

void RandomModule::Finish(){
    for (const auto& it : childs_)
        it.first->Finish();
}

void RandomModule::AddChildModule(AIModule* child, double chance) {
    childs_.emplace_back(std::unique_ptr<AIModule>(child), chance);
    total_probability_ += chance;
    child->set_parent(this);
}

void RandomModule::SetUniformDistribution() {
    total_probability_ = childs_.size() * 1.0;
    for (auto& it : childs_)
        it.second = 1.0;
}

}
