/** @file Units.h
 *  @brief configureable units that encapsulates VPR's packing,
 *  placement, and routing info
 *
 *  @author Mahyar Emami (mayyxeng)
 *  @bug no bugs yet
 *
 */

#ifndef __UNIT_H__
#define __UNIT_H__

#include "Config.h" // for type Config_t

class Block; // forward declaration



/** @brief  AbstractUnit contains the minimum required field for an unit.*/
class AbstractUnit {

public:
  AbstractUnit(){local_coord = Coordinate_t(0, 0);};
  AbstractUnit(Coordinate_t coord):local_coord(coord){};
  /** @brief append a config instruction
   *  @param inst new instruction to append
   */
  void push_back(std::unique_ptr<Instructions::Inst_t> inst);
  
  void print_instructions();
protected:
  Config_t config;
  Coordinate_t local_coord;

};

/** @brief Class declaration for unit of type ConnectionBox.
 *
 *  Each ConnectionBox has a number of track and is connected to a ComputeUnit.
 *  Configuration for each ConnectioBox is set by reading *.route files.
 *  Route files are listing of Nets that are interconnection between nodes in
 *  the dataflow graph generated in the DFGParsing and
 *  BLIF writing phase. Each net starts from either and IOUnit or ComputeUnit.
 *  Then goes through a number of tracks.
 *  As example the net Net 2 is described in *.route as follows:
 *
 *  	Net 2(fork_n6.out3*Fork*~branchC_n4.in1*Fork*
 *
 *		Node:	449	SOURCE (5,4)  Class: 5  Switch: 2
 *		Node:	458	  OPIN (5,4)  Pin: 5   PE_WRAPPER.OUT2[0] Switch: 0
 *		Node:	588	 CHANX (5,3)  Track: 2  Switch: 0
 * 		Node:	744	 CHANY (5,4)  Track: 2  Switch: 0
 * 		Node:	607	 CHANX (5,4)  Track: 1  Switch: 0
 * 		Node:	723	 CHANY (4,4)  Track: 1  Switch: 1
 *		Node:	456	  IPIN (5,4)  Pin: 3   PE_WRAPPER.IN4[0] Switch: 2
 * 		Node:	447	  SINK (5,4)  Class: 3  Switch: -1
 *  This information is parsed and configurations for Connection Boxes at (5,3)
 *  and (4,4) are configured so that track 2 is in ConnectioBox at (5,3) is
 *  connected to ComputeUnit at (5, 4) from OUT2 port and the ConnectionBox at
 *  (4, 4) is connectio to ComputeUnit at (5, 4) from IN4 port.
 *
 */

class ConnectionBox: public AbstractUnit {
public:
  ConnectionBox(){};
  ConnectionBox(Coordinate_t coord) { local_coord = coord; };
private:
};



/** @brief SwitchBox unit declaration.
 *  This class deals with the SwitchBoxes in the overlay.
 *  SwitchBox configurations are inferred by reading *.route files
 *  Take this example of a net:
 *
 *  Node:	588	 CHANX (5,3)  Track: 2  Switch: 0
 *  Node:	744	 CHANY (5,4)  Track: 2  Switch: 0
 *
 *  It means that between ConnectionBox(5,3) and ConnectioBox(5,4)
 *  there is SwitchBox (5, 3) that connections these two tracks.
 *
 */

class SwitchBox: public AbstractUnit {
public:
  SwitchBox(){};
  SwitchBox(Coordinate_t coord) { local_coord = coord; };
private:

};

/** @brief ComputeUnit class inherited from AbstractUnit */
class ComputeUnit: public AbstractUnit {
public:
  ComputeUnit(){};
  ComputeUnit(Coordinate_t coord) { local_coord = coord; };
private:

};


#endif // __UNIT_H__
