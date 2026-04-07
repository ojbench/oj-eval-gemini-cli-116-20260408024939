#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <vector>
#include <string>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  // store the necessary information of the snake
  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  std::vector<std::pair<int, int>> body;
  instruction current_dir;
  
  void initialize(int x, int y, instruction ins){
    // (x,y) isthe position of the head of the snake, ins is the initial orientation of the snake
    //------------------------------------------
    body.clear();
    body.push_back({x, y});
    current_dir = ins;
  }
  int get_length(){
    //return the length of the snake
    //------------------------------------------
    return body.size();
  }
  bool move(Map* map, instruction ins){
    //the current map and the next instruction
    //return false if the snake is dead
    //return true if the snake is alive
    //------------------------------------------
    if (ins == instruction::NONE) {
        ins = current_dir;
    }
    // Check if opposite direction
    if ((ins == instruction::UP && current_dir == instruction::DOWN) ||
        (ins == instruction::DOWN && current_dir == instruction::UP) ||
        (ins == instruction::LEFT && current_dir == instruction::RIGHT) ||
        (ins == instruction::RIGHT && current_dir == instruction::LEFT)) {
        return false; // dead
    }
    current_dir = ins;

    int nx = body[0].first;
    int ny = body[0].second;
    if (ins == instruction::UP) nx--;
    else if (ins == instruction::DOWN) nx++;
    else if (ins == instruction::LEFT) ny--;
    else if (ins == instruction::RIGHT) ny++;

    // Check boundaries
    if (nx < 0 || nx >= get_height(map) || ny < 0 || ny >= get_width(map)) {
        return false;
    }
    // Check wall
    if (is_wall(map, nx, ny)) {
        return false;
    }

    bool ate_food = is_food(map, nx, ny);
    
    // Calculate new body
    std::vector<std::pair<int, int>> new_body;
    new_body.push_back({nx, ny});
    
    int keep_len = ate_food ? body.size() : body.size() - 1;
    for (int i = 0; i < keep_len; ++i) {
        new_body.push_back(body[i]);
    }

    // Check self collision
    for (size_t i = 1; i < new_body.size(); ++i) {
        if (new_body[i].first == nx && new_body[i].second == ny) {
            return false;
        }
    }

    if (ate_food) {
        eat_food(map, nx, ny);
    }

    body = new_body;
    return true;
  }
  std::pair<int, std::pair<int,int>*> get_snake(){
    //return the length of the snake and a pointer to the array of the position of the snake
    //you can store the head as the first element  
    //------------------------------------------
    std::pair<int, int>* arr = new std::pair<int, int>[body.size()];
    for (size_t i = 0; i < body.size(); ++i) {
        arr[i] = body[i];
    }
    return {body.size(), arr};
  }
};

const int MaxWidth = 20;
struct Map{
  // store the necessary information of the map
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;
  int get_height(){
    //return the height of the map
    return height;
  }
  int get_width(){
    //return the width of the map
    return width;
  }
  bool is_food(int x, int y){
    //return true if the cell at (x,y) is food
    return food[x][y];
  }
  void eat_food(int x, int y){
    //eat the food at (x,y)
    food[x][y] = false;
  }
  bool is_wall(int x, int y){
    //return true if the cell at (x,y) is wall
    return wall[x][y];
  }
  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  


  void initialize(Snake *snake){
    char str[MaxWidth];
    int head_x = -1 , head_y = -1;
    instruction ins;
    //------------------------------------------
    std::cin >> height >> width;
    for (int i = 0; i < height; ++i) {
        std::string s;
        std::cin >> s;
        for (int j = 0; j < width; ++j) {
            wall[i][j] = false;
            food[i][j] = false;
            if (s[j] == '#') {
                wall[i][j] = true;
            } else if (s[j] == '*') {
                food[i][j] = true;
            } else if (s[j] == '@') {
                head_x = i;
                head_y = j;
            }
        }
    }
    std::string dir_str;
    std::cin >> dir_str;
    if (dir_str == "U") ins = instruction::UP;
    else if (dir_str == "D") ins = instruction::DOWN;
    else if (dir_str == "L") ins = instruction::LEFT;
    else if (dir_str == "R") ins = instruction::RIGHT;
    else ins = instruction::NONE;
    //------------------------------------------
    snake->initialize(head_x, head_y, ins);
  }
  void print(Snake *snake){
    auto snake_body = snake->get_snake();
    //------------------------------------------
    int len = snake_body.first;
    auto arr = snake_body.second;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            bool is_snake = false;
            bool is_head = false;
            for (int k = 0; k < len; ++k) {
                if (arr[k].first == i && arr[k].second == j) {
                    is_snake = true;
                    if (k == 0) is_head = true;
                    break;
                }
            }
            if (is_head) {
                std::cout << '@';
            } else if (is_snake) {
                std::cout << 'o';
            } else if (wall[i][j]) {
                std::cout << '#';
            } else if (food[i][j]) {
                std::cout << '*';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
    delete[] arr;
  }

};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif
