/**
 * @file /kobuki_driver/src/test/Coordinate.cpp
 *
 * @Map in coordinate.
 **/
#pragma once
/*****************************************************************************
** Includes
*****************************************************************************/
#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include "Coordinate.cpp"
#include "KobukiManager.hpp"
#include "Block.cpp"
/*****************************************************************************
** Define
*****************************************************************************/
#define MINIMUM_SIZE 0.5
#define KOBUKI_SIZE 0.35
/*****************************************************************************
** Classes
*****************************************************************************/

enum Direction{
  RIGHT,
  LEFT,
  UP,
  DOWN,
};

class Map {
public:
  Map(){
    this->max.setCoordinate(0,0);
    this->current_block = NULL;
    //blockListInit();
  }
  Map(double coord_x, double coord_y){
    this->max.setCoordinate(coord_x, coord_y);
    this->current_block = NULL;
    //blockListInit();
  }
  void resize(double coord_x, double coord_y){
    this->max.setCoordinate(coord_x, coord_y);
    this->current_block = NULL;
    blockListInit();
  }

  bool blockListInit(){
    block_list.clear();
    
    int idx_x = this->max.getCoordinateX()/DEFAULT_BLOCK_LENGTH;
    int idx_y = this->max.getCoordinateY()/DEFAULT_BLOCK_LENGTH;
    std::cout << "idx_x = " << idx_x << std::endl; 
    std::cout << "idx_y = " << idx_y << std::endl;
   

    if( idx_x>0 && idx_y>0 ){
      for(int i = 0; i<idx_x; i++) {
      
        std::vector<Block> list;
      
        for(int j = 0; j<idx_y; j++) {
          Coordinate coord;
          coord.setCoordinate(i*DEFAULT_BLOCK_LENGTH, j*(DEFAULT_BLOCK_LENGTH-0.04));
          Block block(coord, DEFAULT_BLOCK_LENGTH, i, j);
  	  list.push_back(block);
        }

        this->block_list.push_back(list);   
        list.clear(); 
      }

      //Edges initialization
      for(int i = 0; i<idx_x; i++) {

        for(int j = 0; j<idx_y; j++) {

	  if(j+1<idx_y) { this->block_list[i][j].edges.up_edge = (Block*) &this->block_list[i][j+1]; }
          else  { this->block_list[i][j].edges.up_edge = NULL; }

	  if(i+1<idx_x) { this->block_list[i][j].edges.right_edge = (Block*) &this->block_list[i+1][j]; }
          else { this->block_list[i][j].edges.right_edge = NULL; }

	  if(j-1>=0) { this->block_list[i][j].edges.down_edge = (Block*) &this->block_list[i][j-1]; }
          else { this->block_list[i][j].edges.down_edge = NULL; }

	  if(i-1<idx_y) { this->block_list[i][j].edges.left_edge = (Block*) &this->block_list[i-1][j]; }
          else  { this->block_list[i][j].edges.left_edge = NULL; }
        }
      }


      this->current_block = (Block*) &block_list[0][0];
      block_list[0][0].setMark(BLANK);
      block_list[0][0].setHasKobuki(true);
      std::cout << "Block list initialezed successfully." << std::endl;
      std::cout << "Size of block_list is [" << this->block_list.size() <<"]" << std::endl;
      return true;
    } else {
      this->current_block = NULL;
      std::cout << "Block list initialeze failed." << std::endl;
      return false;
    }
  }
  
  Block* getNextBlock(){
    
    if(this->block_list.empty()){
      return NULL;
    }
    
    Block* next_block = NULL;
    int now_x = this->current_block->getTagX();
    int now_y = this->current_block->getTagY();
    
    if( now_y-1>=0 && this->block_list[now_x][now_y-1].isUnkonwn() ){
      next_block = &this->block_list[now_x][now_y-1];
    } else if( now_x+1<this->block_list.size() && this->block_list[now_x+1][now_y].isUnkonwn()){
      next_block = &this->block_list[now_x+1][now_y];
    } else if( now_x-1>=0 && this->block_list[now_x-1][now_y].isUnkonwn()){
      next_block = &this->block_list[now_x-1][now_y];
    } else if( now_y+1< this->block_list[now_x].size() && !this->block_list[now_x][now_y+1].isObstacle()){
      next_block = &this->block_list[now_x][now_y+1];
    } else if( now_x-1>=0 ){
      next_block = &this->block_list[now_x-1][now_y];
    } else{
      next_block = NULL;
    }
/*
//////This block can provide a instant value of angle and distance to be advanced///////////

    double dist_x = this->manager.getPose().x() - next_block->getCenterPoint().getCoordinateX();
    double dist_y = this->manager.getPose().x() - next_block->getCenterPoint().getCoordinateY();
    double angle_now = this->manager.getPose().degrees();

    this->angle_to_next = (atan(dist_x/dist_y)/ecl::pi)*180 - angle_now;
    this->distance_to_next = pow( pow(dist_x, 2.0)+pow(dist_y, 2.0), 0.5);
////////////////////////////////////////////////////////////////////////////////////////////
*/
    //std::cout << "Next Position: " << (float)next_block->getCenterPoint().getCoordinateX()
    //    << " , " << (float)next_block->getCenterPoint().getCoordinateY() << std::endl;
    return next_block;
  }

//Get angle will be turned from now to next block
  double getTurnAngle(Block* next_block){
    //Block* next_block = this->getNextBlock();
    if(next_block == NULL){
      return 0;
    }
    
    //double dist_x = this->current_block->centerPoint.getCoordinateX() - next_block->centerPoint.getCoordinateX();
    //double dist_y = this->current_block->centerPoint.getCoordinateX() - next_block->centerPoint.getCoordinateX();
    ecl::Pose2D<double> pose_now = this->manager.getPose();
    Coordinate next_point = next_block->getCenterPoint();
    double dist_x = next_point.getCoordinateX() - pose_now.x();
    double dist_y = next_point.getCoordinateY() - pose_now.y();
    double angle_now = pose_now.heading().degrees();

    double turn_angle = (atan2(dist_y,dist_x)/ecl::pi)*180 - angle_now;

    if((turn_angle < 5.0) && (turn_angle > -5.0)) turn_angle = 0;
    if(turn_angle>180.0) turn_angle = turn_angle - 360.0;
    if(turn_angle<-180.0) turn_angle = turn_angle + 360.0;
    //std::cout << "angle: " << angle_now << std::endl;
    //std::cout << "turn_angle: " << turn_angle << std::endl;
    return turn_angle;
  }

//Get distance will be advaced from now to next block
  double getNextDistance(Block* next_block){

    //Block* next_block = this->getNextBlock();
    if(next_block == NULL){
      return 0;
    }
    
    ecl::Pose2D<double> pose_now = this->manager.getPose();
    Coordinate next_point = next_block->getCenterPoint();
    double dist_x = next_point.getCoordinateX() - pose_now.x();
    double dist_y = next_point.getCoordinateY() - pose_now.y();
    
    double dist = pow( pow(dist_x, 2.0)+pow(dist_y, 2.0), 0.5);
    //std::cout << "x,y,distance: " << dist_x << "," << dist_y << "," << dist << std::endl;

    return dist;
  }

  void getMapInfo(){

    std::cout << "This map has a max coordinate of [" << this->max.getCoordinateX() << "," << this->max.getCoordinateY() << "]" << std::endl;
    if(!block_list.empty()){
      std::cout << "This map has a max block x of [" << this->block_list.size() << "]." << std::endl;
      std::cout << "This map has a max block y of [" << this->block_list.begin()->size() << "]." << std::endl;
    } else {
      std::cout << "This map has no block." << std::endl;
    }
  }
  
  Block* getCurrentBlock(){
    return this->current_block;
  }
  void setCurrentBlock(Block* b){
    this->current_block = b;
    //current_block->setHasKobuki(true);
    return;
  }
  void setMarkBlock(Block* b, Mark m){
    b->setMark(m);
    return;
  }
  KobukiManager* getKobukiManager(){
    return &(this->manager);
  }
  
    void checkWall(){

    int j = 0;
    while(this->block_list[0][j].getMark() != OBSTACLE){
        j++;
    }

    setWall(&this->block_list[0][j]);
  }

  void setWall(Block* wall_block){

  	int idx_x = this->block_list.size();
	int idx_y = this->block_list[0].size();
	int tag_x = wall_block->getTagX();
	int tag_y = wall_block->getTagX();

	if(!(tag_x > 0 && tag_y == 0))
	{
		wall_block->setMark(WALL);

		for(int i = tag_x -1; i<= tag_x + 1; i++)
		{
			if(i == -1) i = 0;
			for(int j = tag_y -1; i<= tag_y + 1; j++)
			{
				if(j == -1) j = 0;
				if(this->block_list[i][j].getMark() == OBSTACLE)
				{
					setWall(&this->block_list[i][j]);
				}
			}
		}
	} 
	else 
	{
		wall_block->setMark(WALL);
	}
      return;
  }

  void showMap(){
    int idx_x = this->block_list.size();
    int idx_y = this->block_list[0].size();
    std::cout << "=====Show now map construct !=====" << std::endl << std::endl;
    for(int i = idx_y-1; i>-1; i--){
      for(int j = 0; j<idx_x ; j++){
        switch(block_list[j][i].getMark()){
          case UNKNOWN: std::cout << "? ";	break;

          case BLANK:	std::cout << "O ";	break;

          case OBSTACLE:std::cout << "X ";	break;
        }
      }
      std::cout << std::endl;
    }
    std::cout << "==================================" << std::endl << std::endl;
    return;
  }

  void showMap(){
    int idx_x = this->block_list.size();
    int idx_y = this->block_list[0].size();
    std::cout << "=====Show now map construct !=====" << std::endl << std::endl;
    for(int i = idx_y-1; i>-1; i--){
      for(int j = 0; j<idx_x ; j++){
        switch(block_list[j][i].getMark()){
          case UNKNOWN: std::cout << "? ";	break;

          case BLANK:	std::cout << "O ";	break;

          case OBSTACLE:std::cout << "X ";	break;
        }
      }
      std::cout << std::endl;
    }
    std::cout << "==================================" << std::endl << std::endl;
    return;
  }

// method for queue

  void pushPath( std::vector<Block *> list ){
    while( !qu_path.empty() ) qu_path.pop();
    for( int i=0; i<list.size(); i++) qu_path.push( list[i] );
    return;
  }

  Block *popPath(){
    Block *pt;
    if( qu_path.empty() ){
      pt = NULL;
    }
    else {
      pt = qu_path.front();
      qu_path.pop();
    }
    return pt;
  }

// private functions

  double getMovement( Block *block ){
    double move = getNextDistance( block );
    move += ( KOBUKI_SIZE / 2 ) - ( block->getSideLength() / 2 );
    return move;
  }

  Direction getDirection(){
    ecl::Pose2D<double> pose = this->manager.getPose();
    double angle = pose.heading().degrees();
    if( (angle <= 45) && ( angle > -45 ) ) return(RIGHT);   // right
    if( (angle <= 135) && ( angle > 45 ) ) return(UP);   // up
    if( (angle <= -135) || ( angle > 135 ) ) return(LEFT); // left
    if( (angle <= -45) && ( angle > -135 ) ) return(DOWN);   // down
  }

  void setObstacleSize( Block *block, double move, Direction direct ){
    // if block has not obstacle object, then create obstacle. 
  }

  void getNearestAndPath( Block* start, std::vector<Block *>& path ){
    
  }

// functions for statemachine

  void updateCurrent(){
    current_block->setHasKobuki(false);
    current_block = next_block;
    current_block->setMark(BLANK);
    current_block->setHasKobuki(true);
    return;
  }

  bool findNext(){

  }

  bool isReached(){
    return( qu_path.empty() );
  }

  void setNext(){
    this->next_block = popPath();
    return;
  }

  void turnToNext( double speed ){
    double turnAngle = getTurnAngle( this->next_block );
    manager.changeDirection( speed, turnAngle );
  }

  void RunToNext( double speed ){
    double runDistance = getNextDistance( this->next_block );
    manager.goStraight( speed, runDistance );
  }

  void foundObstacle(){
    next_block->setMark(OBSTACLE);
    double movement = getMovement( current_block );
    Direction direction = getDirection();
    setObstacleSize( next_block, movement, direction );
  }

  void goBackToCurrent( double speed ){
    next_block = current_block;
    double runDistance = getNextDistance( next_block );
    manager.goStraight( -speed, runDistance );
  }

private:
  Coordinate max;
  std::vector< std::vector<Block> > block_list;
  Block* current_block;
  Block* next_block;
  //double angle_to_next;
  //double distance_to_next;
  KobukiManager manager;
  std::queue<Block *> qu_path;

 };

/*****************************************************************************
** Test Main
*****************************************************************************/

/*
int main(int argc, char* argv[]) {

    Map m(120,200);
    m.blockListInit();
    m.getMapInfo();
    Block* b = m.getNextBlock();
    if(m.getCurrentBlock()!=NULL)std::cout<<"Kobuki is now at block ["<< m.getCurrentBlock()->getTagX() <<","<< m.getCurrentBlock()->getTagY() <<"]."<<std::endl;
    else std::cout<<"Null block position!"<<std::endl;

    if(b!=NULL)std::cout<<"Next block is ["<< b->getTagX() <<", "<< b->getTagY() <<"]."<<std::endl;
    else std::cout<<"Exploration over!"<<std::endl;

    return 0;
}


*/
