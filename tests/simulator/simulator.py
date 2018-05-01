import pygame
import sys
import os
import math
import threading
import traceback
import subprocess
import re

global lock
lock = threading.Lock()

class InputDataReader(threading.Thread):
    
    stop_thread = threading.Event()

    def __init__(self, command, dimension, regex):
        threading.Thread.__init__(self)     # Call constructor of parent
        self.data = [0.0, 0.0, 0.0]
        self.proc = subprocess.Popen([command], stdout=subprocess.PIPE)
        self.dimension = dimension
        self.regex = regex
        print 'Started program {}'.format(command)
        self.start()

    def close(self):
        print 'Process terminate'
        self.proc.terminate()

    def read(self):
        return self.proc.stdout.readline()

    def run(self):
        
        while not self.stop_thread.isSet() :
            data = self.read()                          # Read incoming data
            if re.match(self.regex, data) != None:
                try:
                    values = re.match(self.regex, data).group(1).split(';')
                    if (len(values) == self.dimension):
                        values = [float(i) for i in values]
                        print values
                    else:
                        raise Exception("Bad input length")
                except Exception as e:
                    print "Data input error"
                    print e
                else:
                    lock.acquire()
                    self.data = values
                    lock.release()
            else:
                print data
                print 'Data input does not match regex'

        self.close()

    def stop(self):
        self.stop_thread.set()


#Initilaizing all pygame imported module
pygame.init()

class Display():
    
    SCALE_FACTOR    = 300.0
    BORDER_M        = 0.2
    BORDER_PX       = (int)(BORDER_M * SCALE_FACTOR)
    WIDTH_M         = 3.2
    WIDTH_PX        = (int)(WIDTH_M * SCALE_FACTOR) + (2 * BORDER_PX)
    HEIGHT_M        = 2.0
    HEIGHT_PX       = (int)(HEIGHT_M * SCALE_FACTOR) + (2 * BORDER_PX)
    ROBOT_WIDTH_M   = 0.25
    ROBOT_WIDTH_PX  = (int)(ROBOT_WIDTH_M * SCALE_FACTOR)
    ROBOT_HEIGHT_M  = 0.2
    ROBOT_HEIGHT_PX = (int)(ROBOT_HEIGHT_M * SCALE_FACTOR)
    
    def __init__(self, data_reader):
        self.screen = pygame.display.set_mode((self.WIDTH_PX, self.HEIGHT_PX))
        self.data_reader = data_reader
        self.x = 0
        self.y = 0
        self.theta = 0.0

        try:
            self.run()
        except Exception as e: 
            print(e)
            traceback.print_exc()
            pygame.quit()
            self.data_reader.stop()
            sys.exit()

    def display_grid(self):

        w, h = self.screen.get_size()

        # Draw grid
        for i in range(12):
            if (i == 11):
                width = 3
            else:
                width = 1
                
            pygame.draw.line(self.screen, (150, 150, 150), (0, int(h*0.083333*i)), (w-1, int(h*0.083333*i)), width)

        for i in range(18):
            if (i == 9):
                width = 3
            else:
                width = 1

            pygame.draw.line(self.screen, (150, 150, 150), (int(w*0.0555*i), 0), (int(w*0.0555*i), h-1), width)

    def draw_robot(self, x, y, heading):
    
        diagonal = math.sqrt((float)(self.ROBOT_WIDTH_PX * self.ROBOT_WIDTH_PX + self.ROBOT_HEIGHT_PX * self.ROBOT_HEIGHT_PX))
        diagonal_2 = diagonal / 2.0
        alpha = math.atan(float(self.ROBOT_WIDTH_PX) / float(self.ROBOT_HEIGHT_PX))

        x1 = x + int(diagonal_2 * math.cos(heading + alpha))
        y1 = y + int(diagonal_2 * math.sin(heading + alpha))
        x2 = x + int(diagonal_2 * math.cos(heading - alpha))
        y2 = y + int(diagonal_2 * math.sin(heading - alpha))
        x3 = x + x - x1
        y3 = y + y - y1
        x4 = x + x - x2
        y4 = y + y - y2

        pygame.draw.line(self.screen, (255, 255, 255), (x1, y1), (x2, y2), 3)
        pygame.draw.line(self.screen, (255, 255, 255), (x2, y2), (x3, y3), 3)
        pygame.draw.line(self.screen, (255, 255, 255), (x3, y3), (x4, y4), 3)
        pygame.draw.line(self.screen, (255, 255, 255), (x4, y4), (x1, y1), 3)
        pygame.draw.line(self.screen, (255, 255, 255), (x3, y3), (x,  y),  3)
        pygame.draw.line(self.screen, (255, 255, 255), (x4, y4), (x,  y),  3)

    def update(self, x, y, heading):

        y_pix = int((self.HEIGHT_M - y) * self.SCALE_FACTOR) + self.BORDER_PX
        x_pix = int(((self.WIDTH_M / 2.0) + x) * self.SCALE_FACTOR) + self.BORDER_PX

        self.screen.fill((0, 0, 0))
        self.display_grid()
        self.draw_robot(x_pix, y_pix, -heading)

        pygame.display.update()
        pygame.display.flip()


    def run(self):

        while True:

            #Process events
            event = pygame.event.poll()

            if event.type == pygame.QUIT:
                pygame.quit()
                self.data_reader.stop()
                sys.exit()
            if event.type == pygame.KEYDOWN :
                if event.key == pygame.K_h:
                    self.hold = not self.hold
            
            lock.acquire()
            self.x = self.data_reader.data[0]
            self.y = self.data_reader.data[1]
            self.theta = self.data_reader.data[2]
            lock.release()
            
            self.update(self.x, self.y, self.theta)


pygame.init()

data_reader = InputDataReader(os.path.join(os.curdir, '../../robot'), 3, "\\S+ \\S+\\.cpp:[0-9]+ .+ mesure \\S+ \\S+ (\\S+)")
osc = Display(data_reader)