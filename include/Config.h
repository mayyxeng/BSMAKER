/** @file Config.h
 *  @brief Config type for bit stream generation.
 *  @Author: Mahyar Emami(mayyxeng)
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define MIN(a, b) a <= b ? a : b

// Debuf printf helper macro
#define DEBUG 1
#define DPRINTF(fmt, args...)                                                  \
  do {                                                                         \
    if (DEBUG)                                                                 \
      fprintf(stderr, "%s():%d: " fmt, __func__, __LINE__, ##args);            \
  } while (0)

/** @brief coordiate type for units locations.*/
class Coordinate_t {
public:
  /** @brief constructor for coordnate type
   *  @param x: initial x coordinate
   *  @param y: inital y coordinate
   */
  Coordinate_t(int _x = 0, int _y = 0);

  Coordinate_t operator+(Coordinate_t second);
  Coordinate_t operator-(Coordinate_t second);
  /** @brief copy assignment for Coordinate_t type
   *  @param rhs right hand side of = operator to be copied to left hand side.
   *  @return reference to the newly assigned coordinates
   */
  Coordinate_t &operator=(const Coordinate_t &rhs);
  /** @return returns x value */
  int at_x() const;
  /** @return returns y value */
  int at_y() const;
  /** @param _x sets the x field of coordinates
   *  @return returns the newly set coordinates
   */
  Coordinate_t at_x(int _x);
  /** @param _y sets the y field of coordinate
   *  @return returns the newly set coordinates
   */
  Coordinate_t at_y(int _y);

  std::string tupleStr();

private:
  int x;
  int y;
};

/** config instructions */
namespace Instructions {
enum Opcode { _switch_, _connect_to_, _connect_from_, _set_, _bind_, _null_ };
class Inst_t {
public:
  Inst_t(){};

  /** @brief a function that generates human readable instruction
   *  @return returns a string representing the instruction
   */
  virtual std::string getStr() { return std::string(""); };
  /** @brief opcode getter for instructions
   *  @return returns the Opcode of the instruction
   */
  virtual Opcode getOpcode() = 0;

  virtual Coordinate_t getCoordinates() { return Coordinate_t(0, 0); };

private:
};

/** Switch instruction used for SwitchBox configuration */
class Switch : public Inst_t {
public:
  /** enum class for pin locations */
  enum TLoc { _d0_, _d1_, _d2_, _d3_, _FLOAT_ };

  /** a wrapper for switch instruction operators */
  struct Operand {
    TLoc loc;   // physical location
    int number; // track number
  };
  /** constructor for Switch instruction class */
  Switch(char in_alignment, int in_track, Coordinate_t in_coord,
         char out_alignment, int out_track, Coordinate_t out_coord);
  /** @return Opcode of the instruction */
  Opcode getOpcode() override { return _switch_; };

  /** @brief a function that generates human readable instruction
   *  The string follows the format:
   *    switch ${LOC1}{TRACK1} ${LOC2}{TRACK2} #comment
   *    example : switch N2 S2 #(5,4)
   *  @return returns a string representing the instruction
   */
  std::string getStr() override;

  /** @return coordinates of the switch box
   */
  Coordinate_t getCoordinates() override;

private:
  Operand op1;
  Operand op2;
  Coordinate_t coord;

  /** @brief  Returns the coordinate of the SwitchBox between two channels
   *  @param pos1 position of the first channel
   *  @param pos2 position of the second channel
   *  @return postition of the SwitchBox
   */
  Coordinate_t getSBPosition(Coordinate_t pos1, Coordinate_t pos2);

  std::string LocToStr(TLoc loc);
};

/** Connect instruction class for ConnectionBox configuration*/
class Connect : public Inst_t {
public:
  struct Operand {
    bool in_connection;
    int number;
    std::string name;
  };

  Connect(char pin_dir, std::string pin_name, int pin_idx, Coordinate_t pin_pos,
          char aligment, int track_num, Coordinate_t track_pos);
  /** @return Opcode of the instruction */
  Opcode getOpcode() override { return opcode; };
  std::string getStr() override;
  /** @return coordinates of the switch box
   */
  Coordinate_t getCoordinates() override;
  /** @return 1 if its an input to CU else 0*/
  bool is_input() { return connection_op.in_connection; }

private:
  Switch::Operand switch_op;
  Operand connection_op;
  Coordinate_t connection_coord;
  Coordinate_t switch_coord;
  Opcode opcode;
};

class Bind : public Inst_t {
public:
  struct Operand {
    int index;
    std::string name;
  };

  /** @brief construct the bind instruction
   *  @param component name of the bound component
   *  @param pin_name name of the bound pin
   *  @param pin_index index_number for the pin
   *  @param pos coordinates of the CU
   *  @param dir bind direction, 'O' for outbound and 'I' for inbound
   */
  Bind(std::string component_name, std::string pin_name, int pin_index,
       Coordinate_t pos, char dir);

  /** @return Opcode of the instruction */
  Opcode getOpcode() override { return _bind_; }

  /** @return coordinates of the corresponding CU */
  Coordinate_t getCoordinates() override { return coord; }

  /** @return string representation of the instruction */
  std::string getStr() override;

private:
  Bind::Operand component;
  Bind::Operand pin;
  Coordinate_t coord;
  bool outbound;
};

} // namespace Instructions

/** @brief Represents config for units */
class Config_t {

public:
  Config_t(){};
  void push_back(std::unique_ptr<Instructions::Inst_t> inst);
  void print_instructions();

private:
  // Device specific bit-stream
  unsigned int bit_stream;
  // A set of instructions that specify a config
  std::vector<std::unique_ptr<Instructions::Inst_t>> instructions;
};

#endif // __CONFIG_H__
