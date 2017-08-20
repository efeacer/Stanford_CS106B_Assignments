/**
 * @brief The following code involves the implementations of efficient path finding algorithms
 * such as BFS, Dijkstra and A*. It also involves an algorithm finding an alternative path to a
 * best path.
 * Section Leader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

//I did not write a general search function that can adapt itself to all algorithms because it is
//slightly less efficient (performs a bit more operations, constructs priority queue instead of
//queue etc.). I chose to implement them seperately and specifically.

#include "Trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "hashset.h"
using namespace std;

static const double SUFFICIENT_DIFFERENCE = 0.2;

/**
 * @brief breadthFirstSearch Performs Breadth First Search Algorithm on a graph in order to find
 * the shortest path between two vertexes in terms of the number of travelled vertexes. Returns the
 * shortest path as a collection of its actually travelled vertexes.
 * @param graph A reference to the graph, on which the algorithm functions.
 * @param start A pointer to the starting vertex of the path to search.
 * @param end A pointer to the ending vertex of the path to search.
 * @return The shortest path as a collection of its actually travelled vertexes.
 */
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Queue<Path> paths;
    HashSet<RoadNode*> visited;
    paths.enqueue(Path(1, start));
    start->setColor(Color::YELLOW);
    while (!paths.isEmpty() && !visited.contains(end)) {
        Path possiblePath = paths.dequeue();
        RoadNode* toVisit = possiblePath[possiblePath.size() - 1];
        if (!visited.contains(toVisit)) {
            visited.add(toVisit);
            toVisit->setColor(Color::GREEN);
            if (toVisit == end) {
                return possiblePath;
            }
            for (RoadNode* neighbor: graph.neighborsOf(toVisit)) {
                if (!visited.contains(neighbor)) {
                    paths.enqueue(possiblePath + Path(1, neighbor));
                    neighbor->setColor(Color::YELLOW);
                }
            }
        }
    }
    return Path();
}

/**
 * @brief dijkstrasAlgorithm Performs Dijkstra's Algorithm on a graph in order to find the shortest
 * path between two vertexes in terms of the total cost of travelled edges. Returns the shortest
 * path as a collection of its actually travelled vertexes.
 * @param graph A reference to the graph, on which the algorithm functions.
 * @param start A pointer to the starting vertex of the path to search.
 * @param end A pointer to the ending vertex of the path to search.
 * @return The shortest path as a collection of its actually travelled vertexes.
 */
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    PriorityQueue<Path> paths;
    HashSet<RoadNode*> visited;
    paths.enqueue(Path(1, start), 0);
    start->setColor(Color::YELLOW);
    while (!paths.isEmpty() && !visited.contains(end)) {
        double pathCost = paths.peekPriority();
        Path possiblePath = paths.dequeue();
        RoadNode* toVisit = possiblePath[possiblePath.size() - 1];
        visited.add(toVisit);
        toVisit->setColor(Color::GREEN);
        if (toVisit == end) {
            return possiblePath;
        }
        for (RoadNode* neighbor: graph.neighborsOf(toVisit)) {
            if (!visited.contains(neighbor)) {
                paths.enqueue(possiblePath + Path(1, neighbor),
                              pathCost + graph.edgeBetween(toVisit, neighbor)->cost());
                neighbor->setColor(Color::YELLOW);
            }
        }
    }
    return Path();
}

/**
 * @brief timeHeuristic Returns an admissible heuristic for the cost of a particular edge in the
 * graph. Indeed, the heuristic computes an estimated travel time between two locations by dividing
 * the crow fly distance between the locations to the maximum possible road speed.
 * @param graph A reference to the graph, on which the algorithm functions.
 * @param start A pointer to the starting vertex of the path to search.
 * @param end A pointer to the ending vertex of the path to search.
 * @return An estimated travel time between two locations.
 */
double timeHeuristic(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return graph.crowFlyDistanceBetween(start, end) / graph.maxRoadSpeed();
}

/**
 * @brief aStar Performs A* Algorithm on a graph in order to find the shortest path between two
 * vertexes in terms of the total cost of travelled edges. Uses the power of admissible heuristics
 * to make searching process faster. Returns the shortest path as a collection of its actually
 * travelled vertexes.
 * @param graph A reference to the graph, on which the algorithm functions.
 * @param start A pointer to the starting vertex of the path to search.
 * @param end A pointer to the ending vertex of the path to search.
 * @return The shortest path as a collection of its actually travelled vertexes.
 */
Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    PriorityQueue<Path> paths;
    HashSet<RoadNode*> visited;
    paths.enqueue(Path(1, start), timeHeuristic(graph, start, end));
    start->setColor(Color::YELLOW);
    while (!paths.isEmpty() && !visited.contains(end)) {
        double pathCost = paths.peekPriority();
        Path possiblePath = paths.dequeue();
        RoadNode* toVisit = possiblePath[possiblePath.size() - 1];
        pathCost -= timeHeuristic(graph, toVisit, end);
        visited.add(toVisit);
        toVisit->setColor(Color::GREEN);
        if (toVisit == end) {
            return possiblePath;
        }
        for (RoadNode* neighbor: graph.neighborsOf(toVisit)) {
            if (!visited.contains(neighbor)) {
                paths.enqueue(possiblePath + Path(1, neighbor), pathCost +
                              graph.edgeBetween(toVisit, neighbor)->cost() +
                              timeHeuristic(graph, neighbor, end));
                neighbor->setColor(Color::YELLOW);
            }
        }
    }
    return Path();
}

/**
 * @brief setFromPath Returns a set generated from the given path.
 * @param path The given path, which will be converted to a set.
 * @return The set generated from the path.
 */
HashSet<RoadNode*> setFromPath(Path &path) {
    HashSet<RoadNode*> set;
    for (RoadNode* node: path) {
        set.add(node);
    }
    return set;
}

/**
 * @brief getPathDifference Given two paths, returns the number of elements that are in the first
 * path and not in the second path over the number of elements in the second path.
 * @param path1 The first path.
 * @param path2 The second path.
 * @return The number of elements that are in the first
 * path and not in the second path over the number of elements in the second path.
 */
double getPathDifference(Path &path1, Path &path2) {
    HashSet<RoadNode*> set1 = setFromPath(path1);
    HashSet<RoadNode*> set2 = setFromPath(path2);
    return (set2 - set1).size() / (double) set2.size();
}

/**
 * @brief alternativeRoute Explores an alternative path to the best path of a graph. Uses A* as
 * the search algorithm. Returns an alternative poth if it is the cheapest among all alternatives
 * and it has a sufficient difference from the best path.
 * @param graph A reference to the graph, on which the algorithm functions.
 * @param start A pointer to the starting vertex of the path to search.
 * @param end A pointer to the ending vertex of the path to search.
 * @return The shortest alternative with a sufficient difference from the best path.
 */
Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Path bestPath = aStar(graph, start, end);
    Path bestAlternativePath;
    double minPathCost = 0;
    for (int i = 1; i < bestPath.size(); i++) {
        PriorityQueue<Path> paths;
        HashSet<RoadNode*> visited;
        paths.enqueue(Path(1, start), timeHeuristic(graph, start, end));
        start->setColor(Color::YELLOW);
        while (!paths.isEmpty() && !visited.contains(end)) {
            double pathCost = paths.peekPriority();
            Path possiblePath = paths.dequeue();
            RoadNode* toVisit = possiblePath[possiblePath.size() - 1];
            pathCost -= timeHeuristic(graph, toVisit, end);
            visited.add(toVisit);
            toVisit->setColor(Color::GREEN);
            if (toVisit == end && (minPathCost == 0 || pathCost < minPathCost) &&
                    getPathDifference(possiblePath, bestPath) > SUFFICIENT_DIFFERENCE) {
                bestAlternativePath = possiblePath;
                minPathCost = pathCost;
            }
            RoadEdge* edgeToIgnore = graph.edgeBetween(bestPath[i - 1], bestPath[i]);
            for (RoadNode* neighbor: graph.neighborsOf(toVisit)) {
                RoadEdge* edgeToVisit = graph.edgeBetween(toVisit, neighbor);
                if (!visited.contains(neighbor) && edgeToVisit != edgeToIgnore) {
                    paths.enqueue(possiblePath + Path(1, neighbor), pathCost +
                                  graph.edgeBetween(toVisit, neighbor)->cost() +
                                  timeHeuristic(graph, neighbor, end));
                    neighbor->setColor(Color::YELLOW);
                }
            }
        }
    }
    return bestAlternativePath;
}

