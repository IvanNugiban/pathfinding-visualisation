
#include "resources.h"

namespace engine {

		struct Tile
		{
			sf::Vector2i position{};
			double gCost, hCost{};
			std::shared_ptr<Tile> parent{};

			Tile() = default;

			Tile(sf::Vector2i& pos, double g, double h, std::shared_ptr<Tile> p) : position{pos}, gCost{g}, hCost{h}, parent{p}
			{}

			double fCost() const { return gCost + hCost; };
		};

		enum PathfindingMethod {
			BreadthFirst,
			AStar
		};

		class Grid{
		public:

			Grid(const sf::Vector2f& size, int rows, int columns);

			void render();
			void update();

			void findPath(PathfindingMethod method);

			void clearPath();
			void clearGrid();
			void fillGrid();
			void randomGrid();

			// Events
			void leftClick(sf::Vector2i& mousePos);
			void leftReleased(sf::Vector2i& mousePos);
			void rightClick();
			void rightReleased();

		private:

			// Grid
			sf::RectangleShape m_gridRec{};	
			sf::Vector2f m_gridSize{};

			// Tiles
			std::vector<std::vector<char>> m_tiles{};
			sf::Vector2i m_startTile{};
			sf::Vector2i m_finishTile{};
			sf::Vector2f m_tileSize{};
			int m_rows{};
			int m_columns{};


			// Pathfinding

			// Path to finish
			std::vector <sf::Vector2i> m_path{};
			// All checked tiles
			std::vector <sf::Vector2i> m_checkedTiles{};
			// An array of animated checked tiles
			std::vector<sf::Vector2i> m_checkedTilesAnimation{};

			// Events
			
			// Is user adding obstacles (e.g. left mouse pressed)
			bool m_adding{};

			// Is user removing obstacles (e.g. right mouse pressed)
			bool m_removing{};

			// Is uder dragging	start tile
			bool m_draggingStart{};

			// Is uder dragging	finish tile
			bool m_draggingFinish{};

			// Member functions
			void create(int rows, int columns);
			bool isMouseOverGrid() const;
			sf::Vector2i getTileUnderMouse() const;
			sf::Vector2f getTilePosition(int row, int col) const;

			void drawPath(sf::RectangleShape& tile);
			
			// Breadth first search
			bool isVisited(sf::Vector2i& tile, std::vector<std::vector<bool>>& visited);
			bool isValidTile(sf::Vector2i& tile);
			std::vector<sf::Vector2i> breadthFirstSearch();
			
			// A*
			bool isVisited(sf::Vector2i& tile, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Tile>>>& visited);
			double heuristic(sf::Vector2i curPos, sf::Vector2i endPos);
			std::vector<sf::Vector2i> aStarSearch();
		};

		extern Grid grid;
}