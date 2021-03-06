/**
 * @file /kobuki_driver/src/test/Block.cpp
 *
 * @Coordinate.
 **/
#pragma once
/*****************************************************************************
** Includes
*****************************************************************************/
#include <iostream>
#include "Coordinate.cpp"
/*****************************************************************************
** Define
*****************************************************************************/
#define DEFAULT_BLOCK_LENGTH 0.4
#define DEFAULT_COORD_X 0
#define DEFAULT_COORD_Y 0

/*****************************************************************************
** Enum
*****************************************************************************/
enum Mark
{
	 UNKNOWN,
         BLANK,
         OBSTACLE,
	 WALL,
         SUR_BLANK,
         SUR_OBSTACLE
};

enum IRMark
{
         NOT_EXIST,
         EXIST
};

/*****************************************************************************
** Structs
*****************************************************************************/

struct Borders
{
	float left;
	float right;
	float up;
	float down;

  Borders(){ left = right = up = down = 0.0f; }
};

/*****************************************************************************
** Classes
*****************************************************************************/

class Block {

//Public member
public:
//Constructors
  Block(){
    this->centerPoint.setCoordinate(DEFAULT_COORD_X, DEFAULT_COORD_Y);
    this->side_length = DEFAULT_BLOCK_LENGTH;
    this->mark = UNKNOWN;
    this->irmark = NOT_EXIST;
    this->has_kobuki = false;
    this->tag_x = 0;
    this->tag_y = 0;
    this->borders = NULL;
  }
  Block(double coord_x, double coord_y, double length, int tag_x, int tag_y){
    this->centerPoint.setCoordinate(coord_x, coord_y);
    this->side_length = length;
    this->mark = UNKNOWN;
    this->irmark = NOT_EXIST;
    this->has_kobuki = false;
    this->tag_x = tag_x;
    this->tag_y = tag_y;
    this->borders = NULL;
  }
  Block(Coordinate coord, double length, int tag_x, int tag_y){
    this->centerPoint = coord;
    this->side_length = length;
    this->mark = UNKNOWN;
    this->irmark = NOT_EXIST;
    this->has_kobuki = false;
    this->tag_x = tag_x;
    this->tag_y = tag_y;
    this->borders = NULL;
  }
//Get methods
  Coordinate getCenterPoint(){
    return this->centerPoint;
  }
  float getSideLength(){
    return this->side_length;
  }
  enum Mark getMark(){
    return this->mark;
  }
  enum IRMark getIRMark(){
    return this->irmark;
  }
  bool hasKobuki(){
    return has_kobuki;
  }
  int getTagX(){
    return this->tag_x;
  }
  int getTagY(){
    return this->tag_y;
  }
  bool isUnkonwn(){
    return (this->mark==UNKNOWN?true:false);
  }
  bool isBlank(){
    return (this->mark==BLANK?true:false);
  }
  bool isObstacle(){
    return (this->mark==OBSTACLE?true:false);
  }
//Set methods
  void setMark(enum Mark m){
    this->mark = m;
    return;
  }
  void setIRMark(enum IRMark ir){
    this->irmark = ir;
    return;
  }
  void setHasKobuki(bool b){
    this->has_kobuki = b;
    return;
  }

  struct Borders* borders;

//Private member
private:
  Coordinate centerPoint;
  double side_length;
  enum Mark mark;
  enum IRMark irmark;
  bool has_kobuki;
  int tag_x;
  int tag_y;
};

/*****************************************************************************
** Test Main
*****************************************************************************/
/*
int main() {
    Block b(100,200,40);

    std::cout << "Block [" << b.getCenterPoint().getCoordinateX() << ", " << b.getCenterPoint().getCoordinateY() << "]" << std::endl;
    return 0;
}
*/
