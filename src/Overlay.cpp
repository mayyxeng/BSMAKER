/** @file Overlay.cpp
 *  @brief Overlay methods definitions
 *  @author Mahyar Emami (mayyxeng)
 *
 */
#include "Overlay.h"
#include <iostream>

Block::Block(Coordinate_t coordinates) {
  coord = coordinates;
  CBIn = std::make_unique<ConnectionBox>(Coordinate_t(0, 1));
  CBOut = std::make_unique<ConnectionBox>(Coordinate_t(1, 0));
  SB = std::make_unique<SwitchBox>(Coordinate_t(0, 0));
  CU = std::make_unique<ComputeUnit>(Coordinate_t(1, 1));
}
Coordinate_t Block::getCoordinates() { return coord; }

Overlay::Overlay(int rows, int cols) : rows(rows), cols(cols) {

  DPRINTF("\n\tConstructing an overlay of size %d x %d\n", rows, cols);
  for (int i = 0; i < rows * cols; i++) {
    Block new_block(Coordinate_t(i % cols, i / cols));
    blocks.push_back(std::move(new_block));
  }
  for (int i = 0; i < rows * cols; i++) {
    DPRINTF("Inserted block @ %s\n",
            blocks[i].getCoordinates().tupleStr().c_str());
  }
}

void Overlay::push_back(std::unique_ptr<Instructions::Inst_t> inst) {

  auto inst_coord = inst->getCoordinates();
  int block_index;
  auto opcode = inst->getOpcode();

  switch (opcode) {
  case Instructions::_switch_:
    block_index = inst_coord.at_x() + inst_coord.at_y() * cols;
    break;
  case Instructions::_connect_to_:
    block_index = inst_coord.at_x() + (inst_coord.at_y() - 1) * cols;
    break;
  case Instructions::_connect_from_:
    block_index = inst_coord.at_x() + 1 + inst_coord.at_y() * cols;
    break;
  default:
    block_index = inst_coord.at_x() - 1 + (inst_coord.at_y() - 1) * cols;
  }

  DPRINTF("Found instruction @%s for block %d (%d, %d)\n",
          inst_coord.tupleStr().c_str(), block_index, block_index % cols,
          block_index / cols);
  blocks.at(block_index).push_back(std::move(inst));
}

void Block::push_back(std::unique_ptr<Instructions::Inst_t> inst) {

  auto opcode = inst->getOpcode();

  switch (opcode) {
  case Instructions::_switch_:
    DPRINTF("Appending \'switch\' instruction:\n\t%s\n",
            inst->getStr().c_str());
    SB->push_back(std::move(inst));
    break;
  case Instructions::_connect_to_:
    DPRINTF("Appending \'connect(to)\' instruction :\n\t%s\n",
            inst->getStr().c_str());
    CBIn->push_back(std::move(inst));
    break;
  case Instructions::_connect_from_:
    DPRINTF("Appending \'connect(from)'\ instruction:\n\t%s\n",
            inst->getStr().c_str());
    CBOut->push_back(std::move(inst));
    break;
  case Instructions::_bind_:
    DPRINTF("Appending \'bind\' instructions:\n\t%s\n", inst->getStr().c_str());
    CU->push_back(std::move(inst));
  default:
    break;
  }
}

void Overlay::print_instructions() {

  for (auto iter = blocks.begin(); iter != blocks.end(); iter++) {
    std::cout << "Printing instructions at block "
              << (*iter).getCoordinates().tupleStr() << std::endl;
    (*iter).print_instructions();
  }
}
void Block::print_instructions() {
  SB->print_instructions();
  CBIn->print_instructions();
  CBOut->print_instructions();
  CU->print_instructions();
}
