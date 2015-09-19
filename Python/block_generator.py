#!/usr/bin/python
# -*- coding:utf-8 -*-

import numpy
import Image

class block_agent:
    def __init__(self,field,pos,lifetime=0):
        self.field = field
        self.pos   = pos
        self.lifetime = lifetime
       
    def action(self):
        if(self.field[self.pos[0]][self.pos[1]] == 1):
            return False
        else:
            self.field[self.pos[0]][self.pos[1]] = 1
            if(self.lifetime>0):
                direction = self.decide_direction()
                agent = block_agent(self.field,direction,self.lifetime-1)
                agent.action()

            return True

    def decide_direction(self):
        choices = []
        #pattern = ( (-1,0),(0,-1),(0,1),(1,0) )
        pattern = []

        if self.pos[0]-1 >= 0:
            pattern.append( (-1,0) )
        if self.pos[1]-1 >= 0:
            pattern.append( (0,-1) )
        if self.pos[0]+1 <  self.field.shape[0]:
            pattern.append( (1, 0) )
        if self.pos[1]+1 <  self.field.shape[1]:
            pattern.append( (0, 1) )

        for ptn in pattern:
            location = (self.pos[0]+ptn[0], self.pos[1]+ptn[1] )

            if self.field[location[0]][location[1]] != 1:
                choices.append( (self.pos[0]+ptn[0], self.pos[1]+ptn[1] ) )

        if len(choices)==0:
            return self.pos

        atr = numpy.random.randint(len(choices))

        return choices[atr]

class block_generator:
    def __init__(self,size):
        self.size = size

    def generate(self):
        self.graphic = numpy.zeros(self.size) 

        initial_pos = numpy.random.randint(0,self.size[0]*self.size[1]);
        initial_pos = (initial_pos/self.size[0], initial_pos % self.size[0])

        agent = block_agent(self.graphic,initial_pos,10)
        agent.action()

        if self.block_stable():
            return self.graphic
        else:
            return self.generate()

    def block_stable(self):
        field = self.graphic.copy()

        i=0
        while (field[i/field.shape[0]][i%field.shape[1]]!=0 and i<=field.shape[0]*field.shape[1]):
            i+=1
            
        self.trace_white(field,( i/field.shape[0], i%field.shape[1] ) )

        if numpy.where(field==0)[0].shape != (0,):
            print self.graphic
            print field
            print "This block is not stable!!!!"
            return False
        else:
            return True

    def trace_white(self,field,pos):
        if(field[pos[0]][pos[1]] != 0):
            return
        
        field[pos[0]][pos[1]] = 2

        direction = ( (-1,0), (0,-1), (1,0), (0,1) )

        for d in direction:
            location = [ pos[0]+d[0],pos[1]+d[1] ]

            if location[0] < 0:
                location[0] = field.shape[0]-1
            if location[1] < 0:
                location[1] = field.shape[1]-1
            if location[0] >=  field.shape[0]:
                location[0] = 0
            if location[1] >=  field.shape[1]:
                location[1] = 0

            self.trace_white(field,location)


if __name__ == '__main__':
    generator = block_generator((8,8))
    print generator.generate()

