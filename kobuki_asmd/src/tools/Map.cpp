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
#include "Coordinate.cpp"
#include "KobukiManager.cpp"
#include "Block.cpp"
/*****************************************************************************
** Define
*****************************************************************************/
#define MINIMUM_SIZE 50
/*****************************************************************************
** Classes
*****************************************************************************/

class Map {
public:
  Map(){
    this->max.setCoordinate(0,0);
    this->current_block = NULL;
  }
  Map(int coord_x, int coord_y){
    this->max.setCoordinate(coord_x, coord_y);
    this->current_block = NULL;
  }
  void resize(Coordinate coord){
    this->max = coord;
    this->current_block = NULL;
  }

  bool blockListInit(){

    Coordinate coord;
    int idx_x = this->max.getCoordinateX()/DEFAULT_BLOCK_LENGTH;
    int idx_y = this->max.getCoordinateY()/DEFAULT_BLOCK_LENGTH;
    std::cout << "idx_x = " << idx_x << std::endl; 
    std::cout << "idx_y = " << idx_y << std::endl;
   

    if( idx_x>0 && idx_y>0 ){
      for(int i = 0; i<idx_x; i++) {
      
        std::vector<Block> list;
      
        for(int j = 0; j<idx_y; j++) {
          coord.setCoordinate(20 + i*DEFAULT_BLOCK_LENGTH, 20 + j*DEFAULT_BLOCK_LENGTH);
          Block block(coord, DEFAULT_BLOCK_LENGTH, i, j);
  	  list.push_back(block);
        }

        this->block_list.push_back(list);   
        list.clear(); 
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

    double dist_x = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateX();
    double dist_y = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateY();
    double angle_now = this->kobuki.getPose().degrees();

    this->angle_to_next = (atan(dist_x/dist_y)/ecl::pi)*180 - angle_now;
    this->distance_to_next = pow( pow(dist_x, 2.0)+pow(dist_y, 2.0), 0.5);
////////////////////////////////////////////////////////////////////////////////////////////
*/
    return next_block;
  }

//Get angle will be turned from now to next block
  double getTurnAngle(){
    Block* next_block = this->getNextBlock();
    if(next_block == NULL){
      return 0;
    }
    
    //double dist_x = this->current_block->centerPoint.getCoordinateX() - next_block->centerPoint.getCoordinateX();
    //double dist_y = this->current_block->centerPoint.getCoordinateX() - next_block->centerPoint.getCoordinateX();
    double dist_x = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateX();
    double dist_y = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateY();
    double angle_now = this->kobuki.getPose().degrees();

    double turn_angle = (atan(dist_x/dist_y)/ecl::pi)*180 - angle_now;
    
    return turn_angle;
  }

//Get distance will be advaced from now to next block
  double getNextDistance(){

    Block* next_block = this->getNextBlock();
    if(next_block == NULL){
      return 0;
    }

    double dist_x = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateX();
    double dist_y = this->kobuki.getPose().x() - next_block->centerPoint.getCoordinateY();
    double dist = pow( pow(dist_x, 2.0)+pow(dist_y, 2.0), 0.5);

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
    current_block->setHasKobuki(true);
    return;
  }
private:
  Coordinate max;
  std::vector< std::vector<Block> > block_list;
  Block* current_block;
  double angle_to_next;
  double distance_to_next;
  KobukiManager manager;
 };

/*****************************************************************************
** Test Main
*****************************************************************************/

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

