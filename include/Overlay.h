/** @file Overlay.h
 *  @brief Overlay class is the top hierarchical class that encompasses
 *  all of the blocks
 *
 *  Overlay class takes read the *.route and *.place files then parses the
 *  information into then passes relevant information to the blocks.
 *
 *  @author  Mahyar Emami(mayyxeng)
 */
#ifndef __OVERLAY_H__
#define __OVERLAY_H__

#include "Units.h"
#include <vector>

/** @brief Class declaration for a block.
 *
 *  Each block has:
 *    2 ConnectionBox
 *    1 SwitchBox
 *    1 ComputeUnit
 */

class Block {
public:
  /** @brief Initializes a block at the given coordinates
   *  @param coordinates coordinates of the block in the overlay
   */
  Block(Coordinate_t coordinates);
  /** @return returns the block coordinates */
  Coordinate_t getCoordinates();
  /** @brief pushes an instruction into its corresponding unit
   *  @param inst instruction reference to be inserted
   */
  void push_back(std::unique_ptr<Instructions::Inst_t> inst);

private:
  std::unique_ptr<ConnectionBox> CBIn;
  std::unique_ptr<ConnectionBox> CBOut;
  std::unique_ptr<SwitchBox> SB;
  std::unique_ptr<ComputeUnit> CU;
  Coordinate_t coord;
};

class Overlay {
public:
  /** @brief Initializes the BLOCKS in the overlay
   *  @param rows number of rows of the overlay. The same as VPR.
   *  @param cols number of columns of the overlay. The same as VPR.
   */
  Overlay(int rows, int cols);
  /** @brief pushes back and instruction into the Overlay.
   *  The logical location of the instruction is embedded in the instruction
   *  class and is used here.
   *  @param inst instruction reference to be inserted into the overlay.
   */
  void push_back(std::unique_ptr<Instructions::Inst_t> inst);

private:
  std::vector<Block> blocks;
  int rows;
  int cols;
};

#endif // __OVERLAY_H__
