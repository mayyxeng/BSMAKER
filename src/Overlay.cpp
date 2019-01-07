/** @file Overlay.cpp
 *  @brief Overlay methods definitions
 *  @author Mahyar Emami (mayyxeng)
 *
 */
#include "Overlay.h"

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

  auto sb_coord = inst->getCoordinates();
  // Since unique_ptrs cannot be copied move semantics are used.
  blocks.at(sb_coord.at_x() + sb_coord.at_y() * cols)
      .push_back(std::move(inst));
}

void Block::push_back(std::unique_ptr<Instructions::Inst_t> inst) {
  if (inst->getOpcode() == Instructions::_switch_) {
    DPRINTF("Appending \'switch\' instruction:\n\t%s\n",
            inst->getStr().c_str());
    SB->push_back(std::move(inst));
  } else if (inst->getOpcode() == Instructions::_connect_to_) {
    DPRINTF("Appending \'connect(to)\' instruction :\n\t%s\n",
            inst->getStr().c_str());
    CBIn->push_back(std::move(inst));
  } else if (inst->getOpcode() == Instructions::_connect_from_) {
    DPRINTF("Appending \'connect(from)'\ instruction:\n\t%s\n",
	    inst->getStr().c_str());
    CBOut->push_back(std::move(inst));
  }

}
