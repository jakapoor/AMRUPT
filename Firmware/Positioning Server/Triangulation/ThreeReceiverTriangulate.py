class Coordinate:
    def _init_(self, x = 0.0, y = 0.0):
        self.x = x
        self.y = y

    #Finds intersection of two lines (y = mx + b)
    def intersect (m1, b1, m2, b2):
        intersection = Coordinate()
        xval = float(b2 - b1) / float(m1 - m2)
        yval = xval * m1 + b1
        intersection.x = xval
        intersection.y = yval
        return intersection

    #Finds the centroid of the triangulated area
    def centroid(vertex1, vertex2, vertex3):
        center = Coordinate()
        center.x = (vertex1.x + vertex2.x + vertex3.x) / 3.0
        center.y = (vertex1.y + vertex2.y + vertex3.y) / 3.0
        return center

    #Parameters are decimal angle of arrivals calculated at #each base station and coordinate positions of each #base station.
    def triangulated area (angle_of_arrival1, angle_of_arrival2, angle_of_arrival3, receiver1, receiver2, receiver3):
        #Converts angle degrees to slope
        slope1 = math.tan(angle_of_arrival1) 
        slope2 = math.tan(angle_of_arrival2)
        slope3 = math.tan(angle_of_arrival3)

        b1 = receiver1.y - receiver1.x * slope1 #y = mx + b
        b2 = receiver2.y - receiver2.x * slope2
        b3 = receiver3.y - receiver3.x * slope3

        intersection1 = intersect(slope1, b1, slope2, b2)
        intersection2 = intersect(slope2, b2, slope3, b3)
        intersection3 = intersect(slope1, b1, slope3, b3)

        position = centroid(intersection1, intersection2, intersection3)
        return position
    
