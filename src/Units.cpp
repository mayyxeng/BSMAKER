/** @file Unit.cpp
 *
 *  @author Mahyar Emami (mayyxeng)
 *  @bug no bugs yet
 *
 */



#include "Units.h" // for type Config_t

void
AbstractUnit::push_back(std::unique_ptr<Instructions::Inst_t> inst) {
  config.push_back(std::move(inst));
}

void
AbstractUnit::print_instructions() {
  config.print_instructions();
}
