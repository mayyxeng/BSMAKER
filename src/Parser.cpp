/** @file Parser.cpp
 *  @brief Parser function for *.route and *.place files
 *
 *  The function ParseFiles(const char*) takes the circuit name as
 *  input and parsess the corresponding route and place files then
 *  returns the a configured Overlay class.
 *
 *  @author Mahyar Emami (mayyxeng)
 */
#include "Parser.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

Overlay *ParseFiles(const char *circuit_name) {

  auto route_file = std::string(circuit_name) + ".route";
  auto place_file = std::string(circuit_name) + ".place";
  std::fstream route(route_file);
  if (!route.is_open()) {
    std::cerr << "Error: could not open route file " << route_file << std::endl;
    exit(EXIT_FAILURE);
  }

  Overlay *overlay = nullptr;
  std::regex re_array(
      "^Array size:\\s*(\\d*)\\s*x\\s*(\\d*)\\s*logic blocks.$");
  std::regex re_net("^Net\\s*(\\d*)\\s*\\((.*)\\)$");
  std::regex re_node_pad("^Node:\\s*(\\d*)\\s*(.*)$");
  std::regex re_node_CHAN("^Node:\\s*(\\d*)\\s*CHAN(Y|X)\\s*"
                          "\\((\\d*),(\\d*)\\)\\s*Track: (\\d*) .*$");
  std::regex re_node_block_out(
      "^Node:\\s*(\\d*)\\s*OPIN\\s*\\((\\d*),(\\d*)\\)\\s*Pin:\\s*(\\d*)\\s*("
      "\\w*)\\.(\\w*)\\[(\\d*)\\]\\s*.*$");
  std::regex re_node_block_in(
      "^Node:\\s*(\\d*)\\s*IPIN\\s*\\((\\d*),(\\d*)\\)\\s*Pin:\\s*(\\d*)\\s*("
      "\\w*)\\.(\\w*)\\[(\\d*)\\]\\s*.*$");
  // std::regex re_node_CHANX("^Node:\\s*(\\d*)\\s*(CHANX)
  // \\((\\d*),(\\d*)\\)\\s*Track: (\\d*) .*$");
  /**


  */
  std::string line("");
  std::string prev_line("");
  std::smatch match;
  std::string net("");
  // /std::smatch prev_match;
  parse_state_t state; // Present
  parse_state_t prev_state = _init_;
  while (!route.eof()) {
    getline(route, line);

    state = prev_state;
    if (regex_search(line, match, re_array) && match.size() > 1) {
      DPRINTF("Found Array\n");
      overlay =
          new Overlay(atoi(match.str(1).c_str()), atoi(match.str(2).c_str()));
    }
    if (regex_search(line, match, re_net) && match.size() > 1) {
      // DPRINTF("\n\tFound Net: \n\t%s\n", match.str(2).c_str());
      state = _net_;
      net = match.str(2);
    } else if (regex_search(line, match, re_node_CHAN) && match.size() > 1) {
      // DPRINTF("\n\tFound CHAN%s: \n\t%s\n", match.str(2).c_str(),
      //         match.str(0).c_str());
      state = _chan_;
      std::smatch prev_match;
      if (prev_state == _chan_) {

        regex_search(prev_line, prev_match, re_node_CHAN);

        // DPRINTF("\n\tPrev CHAN%s: \n\t%s\n", prev_match.str(2).c_str(),
        //         prev_match.str(0).c_str());
        Coordinate_t pos1(atoi(prev_match.str(3).c_str()),
                          atoi(prev_match.str(4).c_str()));
        Coordinate_t pos2(atoi(match.str(3).c_str()),
                          atoi(match.str(4).c_str()));
        Instructions::Switch new_switch_inst(
            prev_match.str(2).c_str()[0], atoi(prev_match.str(5).c_str()), pos1,
            match.str(2).c_str()[0], atoi(match.str(5).c_str()), pos2);
        DPRINTF("%s\n", new_switch_inst.getStr().c_str());
        overlay->push_back(
            std::make_unique<Instructions::Switch>(new_switch_inst));

      } else if (prev_state == _blk_out_) {
        regex_search(prev_line, prev_match, re_node_block_out);
        // DPRINTF("\n\tPrev Port:\n\t(%s, %s) @ %s[%s]\n",
        //         prev_match.str(2).c_str(), prev_match.str(3).c_str(),
        //         prev_match.str(6).c_str(), prev_match.str(7).c_str());
        Coordinate_t pin_pos(atoi(prev_match.str(2).c_str()),
                             atoi(prev_match.str(2).c_str()));
        Coordinate_t track_pos(atoi(match.str(3).c_str()),
                               atoi(match.str(4).c_str()));
        Instructions::Connect new_connect_inst(
            'O', prev_match.str(6), atoi(prev_match.str(7).c_str()), pin_pos,
            'X', atoi(match.str(5).c_str()), track_pos);
        DPRINTF("%s\n", new_connect_inst.getStr().c_str());
      }
    } else if (regex_search(line, match, re_node_block_out) &&
               match.size() > 1) {
      // DPRINTF("\n\tFound CU OPin:\n\t%s\n", match.str(0).c_str());
      state = _blk_out_;
    } else if (regex_search(line, match, re_node_block_in) &&
               match.size() > 1) {
      // DPRINTF("\n\tFound CU IPin:\n\t%s\n", match.str(0).c_str());
      state = _blk_in_;
      std::smatch prev_match;
      if (prev_state == _chan_) {
        regex_search(prev_line, prev_match, re_node_CHAN);
        // DPRINTF("\n\tPrev CHAN%s: \n\t%s\n", prev_match.str(2).c_str(),
        //         prev_match.str(0).c_str());
        Coordinate_t track_pos(atoi(prev_match.str(2).c_str()),
                               atoi(prev_match.str(2).c_str()));
        Coordinate_t pin_pos(atoi(match.str(3).c_str()),
                             atoi(match.str(4).c_str()));
        Instructions::Connect new_connect_inst(
            'I', match.str(6), atoi(match.str(7).c_str()), pin_pos, 'Y',
            atoi(prev_match.str(5).c_str()), track_pos);
        DPRINTF("%s\n", new_connect_inst.getStr().c_str());
        overlay->push_back(
            std::make_unique<Instructions::Connect>(new_connect_inst));
      }
    }
    prev_state = state;

    prev_line = std::string(line);
  }
}
