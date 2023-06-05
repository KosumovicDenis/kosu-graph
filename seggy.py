def pathAStar(curr_map, offset: Tuple[int, int], origin: Tuple[int, int], destination: Tuple[int, int]):
            def potential(node: Tuple[int, int]):
                return math.sqrt((origin[0] - node[0]) * 2 + (origin[1] - node[1]) * 2)

            def distance(current: Tuple[int, int], following: Tuple[int, int], rotation: int):
                euclidian_distance = math.sqrt((current[0] - following[0]) * 2 + (current[1] - following[1]) * 2)

                weight = 0

                # Calculate the angle between the current node and the following
                angle = (math.atan2(current[0] - following[0], current[1] - following[1])) % (2 * math.pi)
                if angle > math.pi:
                    angle = 2 * math.pi - angle
                else:
                    angle = -angle
                # Convert the angle to degrees
                angle = (angle * 180 / math.pi)
                # Round the angle to 8th of a circle
                angle = (round(angle / 45) * 45) % 360
                # Get the actual angle considering the cell forward as 0°
                angle = angle - rotation

                if angle == 0:
                    weight += -0.2
                elif angle == 45:
                    weight += .149
                elif angle == 315:
                    weight += 0.15
                elif angle == 90:
                    weight += 0.199
                elif angle == 270:
                    weight += 0.2
                elif angle == 135:
                    weight += 0.249
                elif angle == 225:
                    weight += 0.25
                else:
                    weight += 0.3

                return euclidian_distance + weight

            origin = origin[0] + offset[0], origin[1] + offset[1]
            destination = destination[0] + offset[0], destination[1] + offset[1]

            boundary_nodes = {origin}
            distances = {
                origin: {
                    'distance': 0,
                    'previous': None,
                    'angle': getRotation() * 45
                }
            }

            while len(boundary_nodes) > 0:
                lowest_f_cost = float('inf')
                curr_node = None
                for node in boundary_nodes:
                    if distances[node]['distance'] + potential(node) < lowest_f_cost:
                        lowest_f_cost = distances[node]['distance'] + potential(node)
                        curr_node = node

                boundary_nodes.remove(curr_node)

                ########################################################################################################
                if curr_node == destination:
                    returned_path = copy.copy(set())
                    while distances[curr_node]['previous'] is not None:
                        returned_path.add((curr_node[0] - offset[0], curr_node[1] - offset[1]))
                        curr_node = distances[curr_node]['previous']

                    if distances[destination]['distance'] == 1:
                        destination_potential = 0

                    # Check if the path leads to a hallway, which is preferred by the algorithm
                    elif getRotation() % 2 == 0 and \
                            ((curr_map[destination[0] - 1, destination[1]] == _WALL and
                              curr_map[destination[0] + 1, destination[1]] == _WALL)
                             or
                             (curr_map[destination[0], destination[1] - 1] == _WALL and
                              curr_map[destination[0], destination[1] + 1] == _WALL)):
                        destination_potential = -0.5
                    else:
                        destination_potential = 0

                    return returned_path, distances[destination]['distance'] + destination_potential
                ########################################################################################################

                for shift in {(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)}:
                    neighbour = (curr_node[0] + shift[0], curr_node[1] + shift[1])

                    if curr_map[neighbour[0], neighbour[1]] != _WALL:
                        proposed_distance = distances[curr_node]['distance'] + \
                                            distance(current=curr_node, following=neighbour,
                                                     rotation=distances[curr_node]['angle'])
                        if neighbour not in distances.keys() or distances[neighbour]['distance'] > proposed_distance:
                            distances[neighbour] = {
                                'distance': proposed_distance,
                                'previous': curr_node,
                                'angle': getRotation() * 45
                            }
                            boundary_nodes.add(neighbour)

            print(origin, '->', destination, 'or ', end='')

            origin = origin[0] - offset[0], origin[1] - offset[1]
            destination = destination[0] - offset[0], destination[1] - offset[1]

            print(origin, '->', destination)

            raise ValueError(destination)