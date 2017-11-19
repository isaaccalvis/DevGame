#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "ModuleEnemies.h"


#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

enum ENEMY_TYPES;


class j1PathFinding : public j1Module
{
public:
	j1PathFinding();
	~j1PathFinding();

	bool CleanUp();
	void SetMap(uint width, uint height, uchar* data);
	int CreatePath(const iPoint& origin, const iPoint& destination, ENEMY_TYPES type);

	// To request all tiles involved in the last generated path
	p2DynArray<iPoint>* GetLastPath();

	bool CheckBoundaries(const iPoint& pos) const;

	bool IsWalkable(const iPoint& pos, ENEMY_TYPES type) const;
	uchar GetTileAt(const iPoint& pos) const;

private:
	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created path here
	p2DynArray<iPoint> last_path;
};

struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill, ENEMY_TYPES type) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	int g;
	int h;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	p2List_item<PathNode>* Find(const iPoint& point) const;
	// Returns the Pathnode with lowest score in this list or NULL if empty
	p2List_item<PathNode>* GetNodeLowestScore() const;
	// The list itself, note they are not pointers!
	p2List<PathNode> list;
};



#endif