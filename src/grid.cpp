
#include "../include/grid.h"
#include "../include/window.h"
#include "../include/settings.h"
#include "../include/utils.h"
#include "../include/ui.h"
#include "../include/audio.h"

namespace engine {

	Grid grid{ settings::gridSize, settings::gridRows, settings::gridColumns };

	Grid::Grid(const sf::Vector2f& size, int rows, int columns) : m_gridSize{ size }, m_rows{ rows }, m_columns{ columns }
	{
		// Place the grid at the center at the x coordinate and slightly lower than the center at the y coordinate
		m_gridRec.setPosition({ settings::windowSize.x / 2 - m_gridSize.x / 2, settings::windowSize.y * 0.55f - m_gridSize.y / 2 });
		// Set the size considering the gap
		m_gridRec.setSize({ m_gridSize.x + settings::gridGap, m_gridSize.y + settings::gridGap });
		// Set background color
		m_gridRec.setFillColor(sf::Color::Black);

		// Create tiles
		create(rows, columns);
	}

	void Grid::create(int rows, int columns)
	{
		assert(rows > 0 && columns > 0);

		if (m_tiles.size() > 0) m_tiles.clear();

		/*
			1 represents open paths,
			0 represents obstacles,
			S represents the start point,
			F represents the finish point.
		*/

		for (int x{}; x < rows; x++) {
			std::vector<char> row{};
			for (int y{}; y < columns; y++) {
				row.push_back('1');
			}
			m_tiles.push_back(row);
		}

		// Initialise start and finish tiles
		m_tiles[0][0] = 'S';
		m_tiles[rows - 1][columns - 1] = 'F';


		m_startTile = { 0, 0 };
		m_finishTile = { rows - 1, columns - 1 };

		// Set tile size
		m_tileSize = { m_gridSize.x / m_columns, m_gridSize.y / m_rows };
	}

	bool Grid::isMouseOverGrid() const
	{
		const auto& mousePos = engine::window::mousePos;
		const auto& gridPos = m_gridRec.getPosition();

		return mousePos.x >= gridPos.x &&
			mousePos.x <= gridPos.x + m_gridSize.x &&
			mousePos.y >= gridPos.y &&
			mousePos.y <= gridPos.y + m_gridSize.y;
	}

	// Use only in combination with isMouseOverGrid 
	// P.S: This is because we are returning the desired tile relative to the mouse location, and we need to make sure it is over the grid. 
	// We could do without this, but then we would have to check each tile through a for loop, which is more expensive

	sf::Vector2i Grid::getTileUnderMouse() const
	{
		const auto& mousePos = engine::window::mousePos;

		int relativeX = mousePos.x - m_gridRec.getPosition().x;
		int relativeY = mousePos.y - m_gridRec.getPosition().y;

		sf::Vector2f tileSize = { m_gridSize.x / m_columns, m_gridSize.y / m_rows };

		return {engine::utils::clamp(static_cast<int>(relativeY / tileSize.y), 0, m_rows - 1), 
			engine::utils::clamp(static_cast<int>(relativeX / tileSize.x), 0, m_columns - 1)};
	}

	sf::Vector2f Grid::getTilePosition(int row, int col) const
	{
		return {m_gridRec.getPosition().x + m_tileSize.x * col + settings::gridGap,
			m_gridRec.getPosition().y + m_tileSize.y * row + settings::gridGap};
	}

	void Grid::drawPath(sf::RectangleShape& tile)
	{
		if (m_checkedTiles.size() == 0) {
			return ui::setProcessState(false);
		}

		// Update animation every two animation frames
		if (static_cast<int>(engine::window::animationFrame) >= 2 && m_checkedTilesAnimation.size() < m_checkedTiles.size())  {
			m_checkedTilesAnimation.push_back(m_checkedTiles[m_checkedTilesAnimation.size()]);

			// If we finished, update ui button
			if (m_checkedTilesAnimation.size() == m_checkedTiles.size()) ui::setProcessState(false);

			engine::window::animationFrame = 0;

			// Play beep sound from 0.1 to 4 pitch
			engine::audio::playSound(0.1f + (static_cast<float>(m_checkedTilesAnimation.size()) / m_checkedTiles.size() * 4.0f));
		}

		for (auto& vec : m_checkedTilesAnimation) {

			tile.setFillColor(settings::checkedTileColor);

			tile.setPosition(getTilePosition(vec.x, vec.y));

			engine::window::windowPtr->draw(tile);
		}

		// If we drew all checked tiles
		if (m_checkedTilesAnimation.size() == m_checkedTiles.size())
			for (auto& vec : m_path) {

				tile.setFillColor(settings::pathTileColor);

				tile.setPosition(getTilePosition(vec.x, vec.y));

				engine::window::windowPtr->draw(tile);
			}
	}

	bool Grid::isValidTile(sf::Vector2i& tile)
	{
		return tile.x >= 0 && tile.x < m_tiles.size()
			&& tile.y >= 0 && tile.y < m_tiles[0].size()
			&& m_tiles[tile.x][tile.y] != '0';
	}

	bool Grid::isVisited(sf::Vector2i& tile, std::vector<std::vector<bool>>& visited)
	{
		return !visited[tile.x][tile.y];
	}

	bool Grid::isVisited(sf::Vector2i& tile, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Tile>>>& visited)
	{
		return visited[tile.x][tile.y] != nullptr;
	}

	std::vector<sf::Vector2i> Grid::breadthFirstSearch()
	{
		std::queue<sf::Vector2i> q{};
		q.push(m_startTile);

		std::vector<std::vector<bool>> visited(m_rows, std::vector<bool>(m_columns, false));
		std::vector<std::vector<sf::Vector2i>> parents(m_rows, std::vector<sf::Vector2i>(m_columns, {-1, -1}));
 		visited[m_startTile.x][m_startTile.y] = true;

		while (!q.empty()) {

			sf::Vector2i current = q.front();
			q.pop();

			if (m_tiles[current.x][current.y] == 'F') {

				std::vector<sf::Vector2i> path{};
				sf::Vector2i parent{};

				// While we have parents
				while (parent = parents[current.x][current.y], parent.x != -1) {
					path.push_back(parent);
					current = parent;
				}

				// Remove start tile from list
				path.pop_back();

				// Reverse path to right way
				std::reverse(path.begin(), path.end());

				return path;
			}

			if (m_tiles[current.x][current.y] != 'S') m_checkedTiles.push_back(current);

			// Explore the neighboring cells
			for (int i{}; i < settings::rowDirections.size(); i++)
			{
					int newRow = current.x + settings::rowDirections[i];
					int newCol = current.y + settings::colDirections[i];

					if (isValidTile(sf::Vector2i{ newRow, newCol }) && isVisited(sf::Vector2i{newRow, newCol}, visited)) {
						visited[newRow][newCol] = true;
						parents[newRow][newCol] = current;
						q.push({ newRow, newCol });
					}
			}
		}

		return {};
	}


	double Grid::heuristic(sf::Vector2i curPos, sf::Vector2i endPos)
	{
		return abs(curPos.x - endPos.x) + abs(curPos.y - endPos.y);
	}

	std::vector<sf::Vector2i> Grid::aStarSearch()
	{
		auto compare = [](const std::shared_ptr<Tile> a, const std::shared_ptr<Tile> b) {return a->fCost() > b->fCost() || a->fCost() == b->fCost() && a->hCost > b->hCost; };
		std::priority_queue<std::shared_ptr<Tile>, std::vector<std::shared_ptr<Tile>>, decltype(compare)> q(compare);
		std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Tile>>> visited;
		auto startTile = std::make_shared<Tile>(m_startTile, 0, heuristic(m_startTile, m_finishTile), nullptr);
		visited[m_startTile.x][m_startTile.y] = startTile;
		q.push(startTile);

		while (!q.empty()) {
			auto current = q.top();
			q.pop();

			if (current->position == m_finishTile) {
				std::vector<sf::Vector2i> path{};
				std::shared_ptr<Tile> parent{};

				// While we have parents
				while (parent = current->parent, parent) {
					path.push_back(parent->position);
					current = parent;
				}

				// Remove start tile from list
				path.pop_back();

				// Reverse path to right way
				std::reverse(path.begin(), path.end());

				return path;
			}

			if (m_tiles[current->position.x][current->position.y] != 'S') m_checkedTiles.push_back(current->position);

			for (int i{}; i < settings::rowDirections.size(); i++)
			{
				int newRow = current->position.x + settings::rowDirections[i];
				int newCol = current->position.y + settings::colDirections[i];

				if (!isValidTile(sf::Vector2i{ newRow, newCol })) continue;

				bool tileVisited = isVisited(sf::Vector2i{ newRow, newCol }, visited);

				if (!tileVisited || current->gCost + 1 < visited[newRow][newCol]->gCost) {
					auto newTile = std::make_shared<Tile>(sf::Vector2i{newRow, newCol}, current->gCost + 1, heuristic({newRow, newCol}, m_finishTile), current);
					visited[newRow][newCol] = current;
					q.push(newTile);
				}
			}
		}

		return {};
	}

	void Grid::render()
	{
		// Render grid
		engine::window::windowPtr->draw(m_gridRec);

		// Render tiles
		sf::RectangleShape tile{};

		tile.setSize({ m_tileSize.x - settings::gridGap,
			m_tileSize.y - settings::gridGap });


		for (int row{}; row < m_rows; row++) {
			for (int column{}; column < m_columns; column++) {

				sf::Vector2i currentTile{row, column};

				tile.setPosition(getTilePosition(row, column));

				sf::Color tileColor{};

				switch (m_tiles[row][column]) {
				case '1':
					if (isMouseOverGrid() && getTileUnderMouse() == currentTile) tileColor = settings::tileHoveredColor;
					else tileColor = settings::tileColor;
					break;
				case '0':
					tileColor = settings::tileObstacleColor;
					break;
				case 'S':
					tileColor = settings::startTileColor;
					break;
				case 'F':
					tileColor = settings::finishTileColor;
					break;
				}

				tile.setFillColor(tileColor);
				engine::window::windowPtr->draw(tile);
			}
		}

		// Draw path
		drawPath(tile);

		// If we are dragging a start or finish tile, draw their temporary position on top of the other tiles.
		if (m_draggingStart) {
			tile.setPosition(getTilePosition(m_startTile.x, m_startTile.y));
			tile.setFillColor(settings::startTileColor);
			engine::window::windowPtr->draw(tile);
		}

		if (m_draggingFinish) {
			tile.setPosition(getTilePosition(m_finishTile.x, m_finishTile.y));
			tile.setFillColor(settings::finishTileColor);
			engine::window::windowPtr->draw(tile);
		}
	}
	void Grid::update()
	{
		if (isMouseOverGrid()) {
			auto tile = getTileUnderMouse();
			char tileValue = m_tiles[tile.x][tile.y];

			if (m_draggingStart && tileValue != 'F') {
				m_startTile = tile;
			}

			if (m_draggingFinish && tileValue != 'S') {
				m_finishTile = tile;
			}

			// Add obstacle
			else if (m_adding && tileValue == '1') {
				m_tiles[tile.x][tile.y] = '0';
			}

			// Remove obstacle
			else if (m_removing && tileValue == '0')
				m_tiles[tile.x][tile.y] = '1';
		}
	}

	void Grid::findPath(PathfindingMethod method)
	{
		clearPath();

		std::vector<sf::Vector2i> path{};

		switch (method)
		{
		case BreadthFirst:
			path = breadthFirstSearch();
			break;
		case AStar:
			path = aStarSearch();
			break;
		default:
			break;
		}

		m_path = path;
	}

	void Grid::clearPath()
	{
		m_path.clear();
		m_checkedTiles.clear();
		m_checkedTilesAnimation.clear();
	}

	void Grid::clearGrid()
	{
		ui::setProcessState(false);
		clearPath();

		for (int x{}; x < m_rows; x++) {
			for (int y{}; y < m_columns; y++) {
				if (m_tiles[x][y] != 'S' && m_tiles[x][y] != 'F') {
					m_tiles[x][y] = '1';
				}
			}
		}
	}

	void Grid::fillGrid()
	{
		ui::setProcessState(false);
		clearPath();

		for (int x{}; x < m_rows; x++) {
			for (int y{}; y < m_columns; y++) {
				if (m_tiles[x][y] != 'S' && m_tiles[x][y] != 'F') {
					m_tiles[x][y] = '0';
				}
			}
		}

	}

	void Grid::randomGrid()
	{
		ui::setProcessState(false);
		clearGrid();

		// Get random coverage
		int coverage = rand() % settings::randomGridMaxCoverage + 1;

		for (int x{}; x < m_rows; x++) {
			for (int y{}; y < m_columns; y++) {
				if (m_tiles[x][y] != 'S' && m_tiles[x][y] != 'F' && rand() % 100 + 1 < coverage) {
					m_tiles[x][y] = '0';
				}
			}
		}
	}

	void Grid::leftClick(sf::Vector2i& mousePos)
	{
		if (!m_removing && isMouseOverGrid()) {

			clearPath();
			ui::setProcessState(false);
			auto tile = getTileUnderMouse();
			char tileValue = m_tiles[tile.x][tile.y];

			// Start dragging of start tile
			if (tileValue == 'S') {
				m_tiles[m_startTile.x][m_startTile.y] = '1';
				m_draggingStart = true;
			}

			// Start dragging of finish tile
			else if (tileValue == 'F') {
				clearPath();
				ui::setProcessState(false);
				m_tiles[m_finishTile.x][m_finishTile.y] = '1';
				m_draggingFinish = true;
			}
			// Start adding obstacles
			else m_adding = true;
		}
	}
	void Grid::leftReleased(sf::Vector2i& mousePos)
	{
		if (m_draggingStart) m_tiles[m_startTile.x][m_startTile.y] = 'S';
		if (m_draggingFinish) m_tiles[m_finishTile.x][m_finishTile.y] = 'F';

		m_draggingStart = false;
		m_draggingFinish = false;
		m_adding = false;
	}
	void Grid::rightClick()
	{
		if (!m_adding && isMouseOverGrid()) {

			clearPath();
			ui::setProcessState(false);
			m_removing = true;
		}
	}
	void Grid::rightReleased()
	{
		m_removing = false;
	}
}
