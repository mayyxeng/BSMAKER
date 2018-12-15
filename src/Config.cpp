/** @file Config.cpp
 *  @brief Config and Instruction types for bit stream generation.
 *  @Author: Mahyar Emami(mayyxeng)
 */

#include "Config.h"
#include <sstream>
#include <iostream>
Instructions::Switch::Switch(char in_alignment, int in_track,
                             Coordinate_t in_coord, char out_alignment,
                             int out_track, Coordinate_t out_coord) {

  coord = getSBPosition(in_coord, out_coord);
  Coordinate_t in_diff = in_coord - coord;
  Coordinate_t out_diff = out_coord - coord;
  if (in_alignment == 'X' && out_alignment == 'Y') {
    if (in_diff.at_x() == 1) {
      op1.loc = _d3_;
    } else {
      op1.loc = _d2_;
    }
    if (out_diff.at_y() == 1) {
      op2.loc = _d0_;
    } else {
      op2.loc = _d1_;
    }
  } else if (in_alignment == 'X' && out_alignment == 'X') {
    if (in_diff.at_x() == 1) {
      op1.loc = _d3_;
      op2.loc = _d2_;
    } else {
      op1.loc = _d2_;
      op2.loc = _d3_;
    }

  } else if (in_alignment == 'Y' && out_alignment == 'X') {

    if (in_diff.at_y() == 1) {
      op1.loc = _d0_;
    } else {
      op1.loc = _d1_;
    }
    if (out_diff.at_x() == 1) {
      op2.loc = _d3_;
    } else {
      op2.loc = _d2_;
    }

  } else if (in_alignment == 'Y' && out_alignment == 'Y') {
    if (in_diff.at_y() == 1) {
      op1.loc = _d0_;
      op2.loc = _d1_;
    } else {
      op1.loc = _d1_;
      op2.loc = _d0_;
    }

  } else {
    DPRINTF("\n\tError: Invalid aligment %c -> %c\n", in_alignment,
            out_alignment);
    exit(EXIT_FAILURE);
  }
  op1.number = in_track;
  op2.number = out_track;
}
Coordinate_t Instructions::Switch::getCoordinates() { return coord;}
Coordinate_t Instructions::Switch::getSBPosition(Coordinate_t pos1,
                                                 Coordinate_t pos2) {
  // SB position is (min(pos1.x, pos2.x), min(pos1.y, pos2.y))
  return Coordinate_t(MIN(pos1.at_x(), pos2.at_x()),
                      MIN(pos1.at_y(), pos2.at_y()));
}

std::string Instructions::Switch::getStr() {
  std::string inst = "swtich";
  std::string from = LocToStr(op1.loc) + "@" + std::to_string(op1.number);
  std::string to = LocToStr(op2.loc) + "@" + std::to_string(op2.number);

  inst = inst + " " + from + " " + to + "\t#(" + std::to_string(coord.at_x()) +
         ", " + std::to_string(coord.at_y()) + ")";
  return inst;
}
std::string Instructions::Switch::LocToStr(TLoc loc) {
  switch (loc) {
  case _d0_:
    return std::string("N");
    break;
  case _d1_:
    return std::string("S");
    break;
  case _d2_:
    return std::string("W");
    break;
  case _d3_:
    return std::string("W");
    break;
  }
  return std::string("UNDEF");
}

Instructions::Connect::Connect(char pin_dir, std::string pin_name, int pin_idx,
                               Coordinate_t pin_pos, char aligment,
                               int track_num, Coordinate_t track_pos) {

  connection_op.in_connection = (pin_dir != 'O') ? true : false;
  opcode = (pin_dir != 'O') ? _connect_to_ : _connect_from_;
  connection_op.number = pin_idx;
  connection_op.name = pin_name;
  switch_op.number = track_num;
  connection_coord = pin_pos;
  switch_coord = track_pos;

}

Coordinate_t Instructions::Connect::getCoordinates() { return switch_coord;}

std::string Instructions::Connect::getStr() {
  std::string inst("connect");
  std::string pin = (connection_op.in_connection) ? "in@" : "out@";
  pin += connection_op.name + "@" + std::to_string(connection_op.number);
  std::string track = "track@" + std::to_string(switch_op.number);
  if (connection_op.in_connection) {
    return inst + " " + track + " " + pin + " #" + switch_coord.tupleStr() +
           " -> " + connection_coord.tupleStr();
  } else {
    return inst + " " + pin + " " + track + " #" + connection_coord.tupleStr() +
           " -> " + switch_coord.tupleStr();
  }
}

Coordinate_t::Coordinate_t(int _x, int _y) {
  this->x = _x;
  this->y = _y;
}
int Coordinate_t::at_x() const { return x; }

Coordinate_t Coordinate_t::at_x(int _x) {
  x = _x;
  return (*this);
}

Coordinate_t Coordinate_t::at_y(int _y) {
  y = _y;
  return (*this);
}
int Coordinate_t::at_y() const { return y; }

Coordinate_t Coordinate_t::operator+(Coordinate_t second) {
  return Coordinate_t(x + second.at_x(), y + second.at_y());
}

Coordinate_t Coordinate_t::operator-(Coordinate_t second) {
  return Coordinate_t(x - second.at_x(), y - second.at_y());
}

Coordinate_t &Coordinate_t::operator=(const Coordinate_t &rhs) {

  this->x = rhs.at_x();
  this->y = rhs.at_y();
  return *this;
}
std::string Coordinate_t::tupleStr() {
  return std::string("(" + std::to_string(x) + "," + std::to_string(y) + ")");
}

void
Config_t::push_back(std::unique_ptr<Instructions::Inst_t> inst) {
  instructions.push_back(std::move(inst));
}

void
Config_t::print_instructions(){
  for(auto iter = instructions.begin(); iter != instructions.end(); iter ++){
    std::cout << (*iter)->getStr() << std::endl;
  }
}
