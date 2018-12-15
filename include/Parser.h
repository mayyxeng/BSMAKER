/** @file Parser.h
 *  @brief Parser function for *.route and *.place files
 *  @author  Mahyar Emami(mayyxeng)
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include "Overlay.h"
#include <fstream>
#include <iostream>
#include <regex>



/** @brief Enumaration of parser states */
enum parse_state_t {
  _net_,  // Found net
  _chan_, // Found channel
  _pad_, // Found pad
  _blk_in_, // Found cu
  _blk_out_, // Found cu
  _init_ // Inital state
};

/** @brief ParseFiles reads circuit_name.route and circuit_name.place
 *  files and parses them into Config_t types.
 *
 *  @param cirtcuit_name name of the VPRs output file name without .route
 *         or .place format identifiers.
 *  @return Overlay pointer to the configured Overlay class
 */
Overlay *ParseFiles(const char* circuit_name);

/** @brief  Returns the coordinate of the SwitchBox between two channels
 *  @param pos1 position of the first channel
 *  @param pos2 position of the second channel
 *  @return postition of the SwitchBox
 */
Coordinate_t getSBPosition(Coordinate_t pos1, Coordinate_t pos2);


#endif // __PARSER_H__
