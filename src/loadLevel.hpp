#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include "./Level.hpp"
#include "./logger.h"
#include "./macro.hpp"
#include "glm/glm.hpp"

namespace ost {

Level loadLevel(const char* level_path)
{
    std::vector<glm::vec2> buffer;

        std::ifstream in;
        in.open(level_path, std::ios::binary);

        if (in.fail()) {
            std::cerr << "Unable to open " << level_path << " I'm out!" << std::endl;
            LOG_ERROR("Y out of range in level read");
        }

        // READ LEVEL SIZES
        std::string sizeDefs;
        std::getline(in, sizeDefs);
        std::size_t delimIndex =  sizeDefs.find("x");
        const std::size_t levelWidth = std::stoi(sizeDefs.substr(0, delimIndex));
        const std::size_t levelHeight = std::stoi(sizeDefs.substr(delimIndex+1, sizeDefs.size()-delimIndex+1));


        // Initialize 2d grid vector
        std::vector<std::vector<int>> grid;
        grid.resize(levelHeight);
        for(auto& g : grid) {
            g.resize(levelWidth);
        }
        LOG_DEBUG("%zu  %zu", grid.size(), grid[0].size())

        std::string line;
        for (float y = 0; y < levelHeight ;y++) {
            if (in.eof() || in.bad()) {
                LOG_ERROR("Y out of range in level read");
            }
            
            std::getline(in, line);
            std::stringstream ss(line);

            for (float x = 0; x < levelWidth;x++) {
                int n;
                if(!(ss >> n)) {
                    LOG_ERROR("X out of range while reading level level read");
                } // failed to read, must be end of line



                if(n % 2 == 0) { buffer.push_back(glm::vec2(x, levelHeight - y) ); } // FILL THE BUFFER WITH Vectors - vertex candidate
                
                grid[(levelHeight-1) - y][x] = n;
            }
        }

    Level lvl(buffer, glm::vec2(levelWidth,levelHeight), grid);

    //TODO return levelSize to be able to set the uniform view matrix
    return lvl;
}

}
