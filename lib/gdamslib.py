#!/usr/bin/python
# -*- coding: utf-8 -*-
""" Design of small dams for soil conservation

The MIT License (MIT)

Copyright (c) 2014-2016 Eduardo Jiménez <ecoslacker@irriapps.com>

Permission is hereby granted, free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction,  including without limitation the rights
to use,  copy,  modify,  merge,  publish, distribute, sublicense, and/or sell
copies of the  Software,  and  to  permit  persons to  whom the  Software  is
furnished to do so, subject to the following conditions:

The  above copyright notice and  this permission notice  shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE  WARRANTIES OF MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS  OR  COPYRIGHT  HOLDERS BE  LIABLE FOR  ANY CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF  OR IN CONNECTION  WITH THE  SOFTWARE OR THE USE OR OTHER  DEALINGS IN
THE SOFTWARE.
"""

import csv
import math
import codecs
import pprint
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
# matplotlib.use('Qt4Agg')


class DataFile:
    
    """ Management for stream's cross section data files. """ 
    
    def __init__(self, filename):
        self.filename = filename
        self.data = []
        
    def read_data(self):
        with codecs.open(self.filename, 'r', 'utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
                self.data.append(row)
    
    def check_data(self):
        
        # Check the size of the data array (two columns)
        err = 0
        oks = 0
        for row in self.data:
            if len(row) == 2:
                oks += 1
            else:
                err += 1
        
        if err == 0:
            print("Data array OK!")
        else:
            print("{0} error(s) were found checking size array!".format(err))
        
        # Check the values of the data array
        err = 0
        oks = 0
        for i in range(len(self.data)):
            for j in range(len(self.data[i])):
                try:
                    float(self.data[i][j])
                    oks += 1
                except ValueError:
                    err += 1
        
        if err == 0:
            print("Data values OK!")
            return True
        else:
            print("{0} error(s) were found checking values!".format(err))
            return False

    def show_data(self):
        pprint.pprint(self.data)


class ChannelSection:
    
    """ Channel cross section
    
    Put data in useful lists and draw the cross section.
    
    """
    
    def __init__(self, data, layer_height):
        self.data = data
        self.data_x = []
        self.data_y = []
        self.layers_height = layer_height

        # Initialize some variables
        self.levels = -1
        self.streambed = -1
        self.maxdepth = -1
        self.channel_width = -1
        self.section_type = None
        self.depths = []
        self.widths = []
              
    def get_xy(self):
        
        """ Create two list, one for X data and other for Y data. """

        for row in self.data:
            self.data_x.append(float(row[0]))
            self.data_y.append(float(row[1]))
            
        return self.data_x, self.data_y
            
    def show_xy(self):
        
        """ Show the data lists of X and Y data. """
        
        pprint.pprint(self.data_x)
        pprint.pprint(self.data_y)
        
    def draw_channel(self, path=''):
        
        """ Draw the channel's cross section. """
        
        plt.plot(self.data_x, self.data_y)
        plt.xlabel(u'Ancho (m)')
        plt.ylabel(u'Profundidad (m)')
        plt.title(u'Sección del cauce')
        plt.grid(True)
        plt.gca().invert_yaxis()
        
        # Save fig if there is a path
        if path == '':
            plt.show()
        else:
            plt.savefig(path)
            plt.show()
            
    def get_levels(self, h):
        
        """ Get the levels of the channel using the layer's height. """
        
        # An extra level must be considered for the base, 
        # NOTICE: "self.levels" is NOT the number of layers !!
        # it can be two layers in the same level, e.g. both weir sides
        
        self.levels = int(self.maxdepth / h) + 1
        # return self.levels

    def get_channel_width(self):
        self.channel_width = self.data_x[-1] - self.data_y[0]
    
    def get_maxdepth(self):
        
        """ Get the maximum value of depth of channel's cross section. """
        
        self.maxdepth = max(self.data_y)
        # return self.maxdepth
    
    def get_streambed(self):
        
        """ Get the center of streambed. """
        
        if len(self.widths[0]) == 2:
            self.streambed = self.widths[0][0][0]
        else:
            # Assuming two pairs of coordinates
            diff = (abs(self.widths[0][1][0] - self.widths[0][0][0])) / 2.0
            self.streambed = self.widths[0][0][0] + diff
    
    def find_point(self, x1, x2, y1, y2, d):
        
        """ Find a point using similar triangles

        Given x1, y1 and x2,y2 (base and height of one triangle),
        finds the "x" coordinate for a given "y" between y1 and y2,
        using similar triangles.
        
        """
        
        # Processing large triangle
        base = abs(x2 - x1)
        height = abs(y2 - y1)

        # TODO: This should replace the 'else' conditions below
        # Put false coordinates to initialize
        x = -1
        y = -1

        if y2 == d:
            x = x2
            y = y2
        elif y2 < d:
            # Second point is deeper than current depth
            if y1 > d:
                # Now processing the small triangle
                h = abs(y2 - d)
                b = (base / height) * h
                x = x2 - b
                y = y2 + h
            else:
                # TODO: This 'else' condition should be removed
                # Put false coordinates
                x = -1
                y = -1
        elif y2 > d:
            # First point is deeper than current depth
            if y1 < d:
                # Now processing the small triangle
                h = abs(y1 - d)
                b = (base / height) * h
                x = x1 + b
                y = y1 + h
            else:
                # TODO: This else condition should be removed
                # Put false coordinates
                x = -1
                y = -1
        return x, y
    
    def get_widths(self):
        
        """ Obtains widths of the channel
        
        Return the width of the cross section channel for each
        depth value given.
        
        """
        
        self.get_maxdepth()
        self.get_levels(self.layers_height)
        self.get_channel_width()

        # Start-end coordinates for each layer
        # TODO: Remove this line: self.widths = [(0)] * self.levels
        self.widths = [0] * self.levels
        # self.depths = []
        for i in range(self.levels):
            current_depth = self.maxdepth - (self.layers_height * i)
            self.depths.append(current_depth)
            # print("   Current depth %d: %.3f" % (i, current_depth))
            p = []  # Start-end coordinates for this layer
            for n in range(len(self.data_x)):
                coordx, coordy = self.find_point(self.data_x[n-1],
                                                 self.data_x[n],
                                                 self.data_y[n-1],
                                                 self.data_y[n],
                                                 current_depth)
                if (coordx == -1) and (coordy == -1):
                    # False coordinates detected, ignore them
                    pass
                else:
                    pair = [coordx, coordy]
                    p.append(pair)
            self.widths[i] = p
            try:
                # Obtaining the width for each depth, using the extreme 
                # pair of coordinates
                width = self.widths[i][-1][0] - self.widths[i][0][0]
            except TypeError:
                print("TypeError: Assuming one single point, width = 0")
                width = 0
            except IndexError:
                # Very common, there is only one coordinate pair
                print("IndexError: Assuming one single point, width = 0")
                width = 0
            self.widths[i].append(width)
        # print("Widths array is: %r" % widths)
        
        # Now widths exist and get streambed
        self.get_streambed()
        
        return self.widths
        
    def show_widths(self):
        
        """ Show widths
        
        Display the widths of channel cross section at each depth
        using layer's height.
        
        """
        
        print(" - Width table (in meters):")
        print("  ---------------------------")
        print("   Level\tDepth\tWidth")
        print("  ---------------------------")
        for i in range(len(self.widths)):
            print("   {0}\t{1:0.3f}\t{2:0.3f}".format(i, self.depths[i],
                                                      self.widths[i][-1]))
        print("  ---------------------------")
        # pprint.pprint(self.widths)

    def get_type(self):
        
        """ Get section type
        
        Get the channel's cross section type, it can be 'V', 'U' or 'W', it
        is usefull later to define what kind of algorithms to use for better
        dimensioning.
        
        """
        
        self.section_type = None


class RationalFormula:
    
    """ Runoff calculation using the rational formula
    
    Use the rational formula to obtain the maximum runoff of
    the watershed.
    
    Requires:
        
    -> coefficient : the runoff coefficient [adimensional, no units]
    -> rainfall    : rainfall intensity     [mm h⁻¹]
    -> area        : area of the watershed  [ha]
    
    Returns:
    
    -> runoff      : watershed runoff       [m³ s⁻¹]
    
    """
    
    def __init__(self, c=0.0, i=0.0, a=0.0):
        self.coefficient = float(c)   # Runoff coefficient
        self.rainfall = float(i)      # Rainfall intensity
        self.area = float(a)
        self.runoff = None
        
    # def set_runoff_coeficient(self, rc):
    #     self.coefficient = float(rc)
    #
    # def set_rainfall_intensity(self, ri):
    #     self.rainfall = float(ri)
    #
    # def set_area(self, a):
    #     self.area = float(a)
        
    def get_runoff(self):
        self.runoff = (self.coefficient * self.rainfall * self.area) / 360.0
        return self.runoff


class HighWaterMarks(ChannelSection):
    """ Use the high water marks method to get the maximum runoff of
        the stream.
    """
    
    def __init__(self, water_height):
        self.water_height = water_height


class Stream:
    
    """ Stream characteristics, like length, slope, etc. """
    
    def __init__(self, rise, run):
        self.rise = float(rise)
        self.run = float(run)
        self.slope = 0
        
    # def set_slope(self, slope):
    #     self.slope = float(slope)
        
    def get_slope(self):
        self.slope = self.rise / self.run
        return self.slope


class RunoffCoefficient:
    """ Runoff coefficient, using land use, soil type, and precipitation. """
    
    def __init__(self, landuse, soiltype, precipitation):
        
        self.land_use = int(landuse)
        self.soil_type = int(soiltype)
        self.precipitation = float(precipitation)
        
        # From this table we can obtain a factor named "k", that is
        # needed to runoff coefficient. Land use and soil type are needed.
        self.table = [[0.26, 0.28, 0.30], [0.24, 0.27, 0.30], 
                      [0.24, 0.27, 0.30], [0.24, 0.27, 0.30], 
                      [0.14, 0.20, 0.28], [0.20, 0.24, 0.30],
                      [0.24, 0.28, 0.30], [0.07, 0.16, 0.24], 
                      [0.12, 0.22, 0.26], [0.17, 0.26, 0.28], 
                      [0.22, 0.28, 0.30], [0.26, 0.29, 0.32],
                      [0.27, 0.30, 0.33], [0.18, 0.24, 0.30]]

        self.k = -1
        self.lu = []
        self.st = []
        self.coefficient = -1
                      
    def show_land_use(self):
        
        self.lu = [u"0 = Barbecho, áreas incultas y desnudas",
                   u"1 = Cultivos en hilera",
                   u"2 = Legumbres o rotación de pradera",
                   u"3 = Granos pequeños",
                   u"4 = Pastizal (75%)",
                   u"5 = Pastizal (50-75%)",
                   u"6 = Pastizal (50%)",
                   u"7 = Bosque (75%)",
                   u"8 = Bosque (50-75%)",
                   u"9 = Bosque (25-50%)",
                   u"10 = Bosque (25%)",
                   u"11 = Zona urbana",
                   u"12 = Caminos",
                   u"13 = Pradera permanente"]

        pprint.pprint(self.lu)

    def show_soil_type(self):
        
        self.st = [u"0 = Suelo permeable",
                   u"1 = Suelo de permeabilidad media",
                   u"2 = Suelo casi impermeable"]

        pprint.pprint(self.st)
    
    def show_table(self):
        pprint.pprint(self.table)
    
    def get_coefficient(self):
        
        # Selecting the k factor for runoff coefficient
        self.k = self.table[self.land_use][self.soil_type]

        if self.k <= 0.15:
            self.coefficient = self.k * (self.precipitation - 250.0) / 2000.0
        elif self.k > 0.15:
            self.coefficient = self.k * (self.precipitation - 250.0) / 2000.0 + (self.k - 0.15) / 1.5
        return self.coefficient

    def calc_runoff_coefficient(self, k, precipitation):
        coefficient = -1
        if k <= 0.15:
            coefficient = k * (precipitation - 250.0) / 2000.0
        elif k > 0.15:
            coefficient = k * (precipitation - 250.0) / 2000.0 + (k - 0.15) / 1.5
        return coefficient


class ConcentrationTime:
    
    """ Concentration time of the watershed. """
    
    def __init__(self, area, length, rise):
        self.area = float(area)
        self.length = float(length)
        self.rise = float(rise)

        self.concentration_time = -1
        
    def get_concentration_time(self):
        if self.area >= 200:
            # print(" - (Using Kirpich equation for concentration time)")
            # Use Kirpich equation 
            self.concentration_time = ((0.000325 * (self.length ** 0.77) / ((self.rise / self.length) ** 0.385)) * 60)
        elif self.area < 200:
            # Use Giandotti equation
            # Remember: 100 ha = 1 km²
            # print(" - (Using Giandotti equation for concentration time)")
            # print("Area: %.3f ha = %.3f km²" % (self.area, self.area/100))
            # print("Length of main stream: %.3f " % self.length)
            t1 = 4 * math.sqrt(self.area / 100.0)
            t2 = 1.5 * (self.length / 1000.0)
            t3 = math.sqrt((self.rise / self.length) * (self.length / 1000.0))
            self.concentration_time = (((t1 + t2) / (25.3 * t3)) * 60.0)
        
        return self.concentration_time


class Weir:
    
    """ Weir attributes and dimensioning. """
    
    def __init__(self, q=0, c=1.45, l=0, h=0, n='h', lh=0.5):
        self.flowrate = q     # flow rate
        self.weir_coefficient = c  # weir coefficient for structure
        self.weir_length = l       # width of the crest
        self.weir_head = h         # height of water head over the crest
        self.notch_shape = n  # notch shape
        self.layers_height = lh
        self.weir_height = -1
        self.weir_layers = -1
        self.notch()

        self.weir_notch = -1

    def notch(self):
        """ Get exponent: 'h' for horizontal weir or 'v'-notch. """

        if self.notch_shape == 'h':
            self.weir_notch = 3. / 2.
        elif self.notch_shape == 'v':
            self.weir_notch = 5. / 2.

    def get_flowrate(self):
        """ Get the value of flow rate passing through weir. """

        self.notch()

        self.flowrate = self.weir_coefficient * self.weir_length * pow(self.weir_head, self.weir_notch)
        return self.flowrate
        
    def get_length(self):
        """ Get the weir length given head of water and flow rate. """

        self.notch()

        self.weir_length = pow(self.flowrate / (self.weir_coefficient * self.weir_head), 1.0 / self.weir_notch)
        return self.weir_length
        
    def get_head(self):
        """ Get the height of water head over the crest. """

        self.notch()

        self.weir_head = pow(self.flowrate / (self.weir_coefficient * self.weir_length), 1.0 / self.weir_notch)
        return self.weir_head

    def get_height(self):
        """ Get the actual weir height according the layer's height used. """

        self.notch()
        self.get_head()

        # Weir height
        self.weir_height = 0
        self.weir_layers = 0  # weir layers
        while self.weir_height <= self.weir_head:
            self.weir_layers += 1
            self.weir_height += self.layers_height
        self.print_weir()
        return self.weir_height

    def print_weir(self):
        print("Weir data:")
        print(" Flow rate:     {0}".format(self.flowrate))
        print(" Coefficient:   {0}".format(self.weir_coefficient))
        print(" Length:        {0}".format(self.weir_length))
        print(" Head:          {0}".format(self.weir_head))
        print(" Notch:         {0}".format(self.notch_shape))
        print(" Layers height: {0}".format(self.layers_height))
        print(" Height:        {0}".format(self.weir_height))
        print(" Layers:        {0}".format(self.weir_layers))
        print(" n:             {0}".format(self.weir_notch))

class Weir2:
    
    """ Weir attributes and dimensioning. """
    
    def __init__(self, q=0, c=1.45, l=0, h=0, n='h'):
        self.flow_rate = q    # flow rate
        self.constant = c     # constant for structure
        self.weir_length = l  # width of the crest or weir length
        self.head = h         # height of head of water over the crest
        self.notch = n        # notch shape
        self.get_weir_n()

        self.n = -1
    
    def get_weir_n(self):
        
        """ Get exponent: 'h' for horizontal weir or 'v'-notch. """
        
        if self.notch == 'h':
            self.n = 3./2.
        elif self.notch == 'v':
            self.n = 5./2.
            
    def get_weir_flow_rate(self, l, h, c=1.45, n='h'):
        
        """ Get the value of flow rate passing through weir. """
        
        self.constant = c  # constant for structure
        self.weir_length = l   # width of the crest or weir length
        self.head = h      # height of head of water over the crest
        self.notch = n     # notch shape
        
        self.get_weir_n()
        
        self.flow_rate = self.constant * self.weir_length * pow(self.head, self.n)
        return self.flow_rate
        
    def get_weir_length(self, q, h, c=1.45, n='h'):
        
        """ Get the weir length given head of water and flow rate. """
        
        self.flow_rate = q  # flow rate
        self.constant = c   # constant for structure
        self.head = h       # height of head of water over the crest
        self.notch = n      # notch shape
        
        self.get_weir_n()

        self.weir_length = pow(self.flow_rate / (self.constant * self.head),
                               1.0 / self.n)
        return self.weir_length
        
    def get_weir_head(self, q, l, c=1.45, n='h'):
        
        """ Get the height of head of water over the crest. """
        
        self.flow_rate = q  # flow rate
        self.constant = c   # constant for structure
        self.weir_length = l   # width of the crest
        self.notch = n      # notch shape
        
        self.get_weir_n()
        
        self.head = pow(self.flow_rate / (self.constant * self.weir_length), 1.0 / self.n)
        return self.head


class GabionDam(ChannelSection, Weir):
    
    """ Body of the dam. """
    
    def __init__(self, section_data, layer_height, flowrate, weir_coef, weir_length, water_height, notch):
        ChannelSection.__init__(self, section_data, layer_height)
        self.weir = Weir.__init__(self, flowrate, weir_coef, weir_length, water_height, notch)
        
        # Table of dimensions of the dam
        self.dimensions = []

        self.abutment = -1
                
    def auto_dimensioning(self):
        """ Automatic dimensions

        Generates automatic dimensions for each dam's layer using widths
        list, layer's height, abutment and weir length.
        """

        # Use the abutment as same as layer's height
        self.abutment = self.layers_height
        print(" Abutment: {0}".format(self.abutment))

        print("Dimension layers per level:")

        # Automatic generated dimensions list: Length, Width, Height, X, Y
        auto_dimensions = []
        for i in range(self.levels):
            print("LEVEL {0}".format(i))
            
            # The dam is going to have a 'square' form, it is an equal
            # length and height, so get their maximum dimension
            max_dim = self.levels * self.layers_height
            # Current depth of each level
            d = self.maxdepth - (self.layers_height * i)
            # Set the length value of each layer
            l = max_dim - (self.layers_height * i)
            # Set the width of each layer
            w = self.widths[i][-1] + (self.abutment * 2)
            # Set a constant value for each layer
            h = self.layers_height
            # Set the (x,y) coordinates of left bottom vertex (in front view)
            x = self.widths[i][0][0] - self.abutment
            y = self.widths[i][0][1] + self.layers_height
            # y = d + self.layers_height

            print(" Current depth: {0}".format(d))
            print(" Length: {0}".format(l))
            print(" Width: {0}".format(w))
            print(" Height: {0}".format(h))
            print(" X: {0} Y: {1}".format(x, y))
            
            # Finally append the automatic generated dimensions
            auto_dimensions.append([l, w, h, x, y])
            
            # print(u"   {0} d={1} l={2}, w={3:.2f}, h={4}, x={5:.2f}, y={6:.2f}".format(i, d, l, w, h, x, y))
        self.dimensions = auto_dimensions

    def dimension_weir(self):
        """Create the weir dimensions"""

        print("Creating weir dimensions")

        # Get the possible limits of the weir, this get the layer that the weir bottom can reach.
        weir_bottom = self.levels - (self.weir_layers + 1)
        print(self.levels)
        print(self.weir_layers)

        self.weir_left_limit = self.widths[weir_bottom][0][0]
        self.weir_right_limit = self.widths[weir_bottom][-2][0]

        print(" Weir bottom layer: {0}".format(weir_bottom))
        print(" Weir left limit:   {0}".format(self.weir_left_limit))
        print(" Weir right limit:  {0}".format(self.weir_right_limit))

    def create_weir(self):
        
        """ Creates a weir using the last(s) layer(s) of the dam. """
        
        # The weir should be located at the center of the streambed, or at
        # least cover it.
        
        # The layer that will be splitted to weir walls
        layer = -1  # Last layer
        
        # Check weir length dimensions
        if self.weir_length >= self.dimensions[layer][1]:
            print("ERROR: Cannot create weir, excessive dimensions!")
            return 1
        
        # Getting X coordinate of weir position, try centering to streambed
        weir_x = self.streambed - (self.weir_length / 2.0)
        
        # Check if the channel's width allows the weir being centered 
        if weir_x < self.dimensions[layer][2]:
            print(" -Weir overflows by left side!")
        if (weir_x + self.weir_length) > (self.dimensions[layer][2] + self.dimensions[layer][1]):
            print(" -Weir overflows by right side!")

        rightwall_width = self.dimensions[layer][1] - self.dimensions[0][1] - weir_x

        # Creates the left wall of the weir by resizing the last layer
        # self.dimensions[layer][1] = (self.dimensions[layer][3] + self.dimensions[layer][1]) - weir_x
        #TODO: FIX THIS!
        #TODO: FIX THIS!
        self.dimensions[layer][1] = self.dimensions[layer][1] - rightwall_width - self.dimensions[0][1]
        
        # Creates the right wall of the weir as a new layer
        rightwall_length = self.dimensions[layer][0]
        #rightwall_width = self.dimensions[layer][3] + self.dimensions[0][3]
        rightwall_height = self.dimensions[layer][2]
        rightwall_x = self.dimensions[0][3] + self.dimensions[0][1]
        rightwall_y =  self.dimensions[layer][4]

        right_wall = []
        right_wall.append(rightwall_length)
        right_wall.append(rightwall_width)
        right_wall.append(rightwall_height)
        right_wall.append(rightwall_x)
        right_wall.append(rightwall_y)

        self.dimensions.append(right_wall)
        
    def show_dimensions(self):
        
        """ Show the dimensions of the dam. """
        
        # pprint.pprint(self.dimensions)
        print(u" - Dimensions table (in meters):")
        print(u"  -----------------------------------------------------------")
        print(u"   Layer\tLength\tWidth\tHeight\tX\tY")
        print(u"  -----------------------------------------------------------")
        for i in range(len(self.dimensions)):
            print(u"   {0}\t{1:0.2f}\t{2:0.2f}\t{3:0.2f}\t{4:0.2f}\t{5:0.2f}".format(i,
                                                                                     self.dimensions[i][0],
                                                                                     self.dimensions[i][1],
                                                                                     self.dimensions[i][2],
                                                                                     self.dimensions[i][3],
                                                                                     self.dimensions[i][4]))
        print(u"  -----------------------------------------------------------")
            
    def front_view(self, x=None, y=None, filename=''):
        
        """ Draws the front view of the dam. """
        
        fig = plt.figure()
        for i in range(len(self.dimensions)):
            left = self.dimensions[i][3]
            right = self.dimensions[i][3] + self.dimensions[i][1]
            # Because the inverted axis bottom and top should be as follow
            top = self.dimensions[i][4]
            bottom = self.dimensions[i][4] - self.dimensions[i][2]
            
            verts = [(left, bottom),
                     (left, top),
                     (right, top),
                     (right, bottom),
                     (left, bottom)]
            
            codes = [Path.MOVETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.CLOSEPOLY]
                     
            path = Path(verts, codes)
            ax = fig.add_subplot(111)
            patch = patches.PathPatch(path, fc='white', lw=1, hatch='x')
            ax.add_patch(patch)
        # Plot the channel's cross section from argument, if any
        if x is not None and y is not None:
            plt.plot(x, y)
        ax.axis('equal')
        plt.xlabel(u'Width (m)')
        plt.ylabel(u'Depth (m)')
        plt.title(u'Front view')
        plt.grid(True)
        plt.gca().invert_yaxis()
        
        # Save fig if there is a path
        if filename == '':
            plt.show()
        else:
            plt.savefig(filename)
            plt.show()

    def side_view(self, filename=''):
        
        """ Draws the side view of the dam. """
        
        fig = plt.figure()
        for i in range(len(self.dimensions)):
            left = 0
            right = self.dimensions[i][0]
            # Because the inverted axis bottom and top should be as follow
            top = self.dimensions[i][4]
            bottom = self.dimensions[i][4] - self.dimensions[i][2]
            
            verts = [(left, bottom),
                     (left, top),
                     (right, top),
                     (right, bottom),
                     (left, bottom)]
            
            codes = [Path.MOVETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.CLOSEPOLY]
                     
            path = Path(verts, codes)
            ax = fig.add_subplot(111)
            patch = patches.PathPatch(path, fc='white', lw=1, hatch='x')
            ax.add_patch(patch)
        ax.axis('equal')
        plt.xlabel(u'Length (m)')
        plt.ylabel(u'Depth (m)')
        plt.title(u'Side view')
        plt.grid(True)
        plt.gca().invert_yaxis()
        
        # Save fig if there is a path
        if filename == '':
            plt.show()
        else:
            plt.savefig(filename)
            plt.show()
    
    def top_view(self, filename=''):
        
        """ Draws the top view of the dam. """
        
        fig = plt.figure()
        for i in range(len(self.dimensions)):
            left = self.dimensions[i][3]
            right = self.dimensions[i][3] + self.dimensions[i][1]
            # Because the inverted axis bottom and top should be as follow
            top = 0
            bottom = self.dimensions[i][4] - self.dimensions[i][2]
            
            verts = [(left, bottom),
                     (left, top),
                     (right, top),
                     (right, bottom),
                     (left, bottom)]
            
            codes = [Path.MOVETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.CLOSEPOLY]
                     
            path = Path(verts, codes)
            ax = fig.add_subplot(111)
            patch = patches.PathPatch(path, fc='white', lw=1, hatch='x')
            ax.add_patch(patch)
        ax.axis('equal')
        plt.xlabel(u'Width (m)')
        plt.ylabel(u'Length (m)')
        plt.title(u'Top view')
        plt.grid(True)
        plt.gca().invert_yaxis()
        
        # Save fig if there is a path
        if filename == '':
            plt.show()
        else:
            plt.savefig(filename)
            plt.show()


class Stability(GabionDam):
    
    """ Oropeza's method for stability analysis
    
    Stability analysis for small gabion dams for soil conservation
    using the methodology of Oropeza-Mota J. L., 2009 (reviewed at 2013).
    According to notes taken from class of Soil Conservation at the
    Irrigation Department from Chapingo Autonomous University (http://www.chapingo.mx)
    
    """
    
    def __init__(self):
        pass
    
# END OF THE FUNCTIONS

if __name__ == "__main__":
    
    # Choose a layer's height
    heights = [0.15, 0.30, 0.5, 1.0]

    # DATA AND DEFINITIONS
    
    # Open the file that contain data
    files = ['/home/ecoslacker/Dropbox/Projects/Gabbioni/examples/data/test_data_01.csv']
    
    for datafile in files:

        # Rational method
        land_use = 8      # land use (0-13), use show_land_use()
        soil_type = 2     # soil type (0-2), use show_soil_type()
        rainfall = 809.1  # annual precipitation, in mm
        intensity = 111   # rainfall intensity, in mm/h
        wshd_area = 45    # watershed area, in ha
        str_length = 989  # stream length, in m
        str_rise = 23     # stream rise (elevation difference), in m
    
        # Weir
        weir_len = 2
        weir_c = 1.45
        weir_notch = 'h'
        weir_head = 0  # height of water head, to be calculated
        
        # OPERATIONS AND CALCULATIONS
        
        # Datafile
        fdata = DataFile(datafile)
        fdata.read_data()
        fdata.check_data()
        
        # Rational method
        rc = RunoffCoefficient(land_use, soil_type, rainfall)
        runoff_coef = rc.get_coefficient()
        rational = RationalFormula(runoff_coef, intensity, wshd_area)  # max runoff
        ct = ConcentrationTime(wshd_area, str_length, str_rise)
        conc_time = ct.get_concentration_time()
        runoff = rational.get_runoff()
    
        # Create a gabion dam
        hlayer = heights[2]  # layer's height
        dam = GabionDam(fdata.data, hlayer, runoff, weir_c, weir_len, weir_head, weir_notch)
        dam.get_xy()
        dam.get_widths()
        
        # Weir
        dam.get_head()
        # weir_length = dam.get_weir_length(q, weir_head)  # get weir length
        dam.auto_dimensioning()
        # if dam.autodimensioning() != 0:
        #     print("ERROR: Cannot create dimensions!")
        #     break
        
        # SHOW RESULTS

        print(u"Data")
        fdata.show_data()
        
        print(u"Rational method")
        print(u" - Maximum runoff:     {0:.3f} m³/s".format(runoff))
        print(u" - Runoff coefficient: {0:.3f}".format(runoff_coef))
        print(u" - Concentration time: {0:.3f} min.".format(conc_time))
    
        print("Channel's cross section")
        print(" - Layer's height:  {0:.2f} m".format(dam.layers_height))
        print(" - Channel levels:  {0}".format(dam.levels))
        print(" - Channel's width: {0:.2f} m".format(dam.channel_width))
        print(" - Channel's depth: {0:.2f} m".format(dam.maxdepth))
        print(" - Streambed at x:  {0:.2f} m".format(dam.streambed))
        dam.show_widths()
        print(dam.widths)
        # dam.show_xy()
        # dam.draw_channel('/home/eduardo/seccion.png')
    
        print(u"Weir")
        print(u" - Coefficient: {0:.2f}".format(dam.weir_coefficient))
        print(u" - Water head:  {0:.2f} m".format(dam.weir_head))
        print(u" - Length:      {0:.2f} m".format(weir_len))
        
        print("Dam")
        dam.show_dimensions()
        dam.front_view(dam.data_x, dam.data_y)
        dam.side_view()
        dam.top_view()

        # STABILITY ANALYSIS
