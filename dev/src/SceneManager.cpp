#include "SceneManager.h"

// Static controllers for mouse and keyboard
static bool keys[1024];
static bool keylock[1024];
static bool resized;
static GLuint width, height;

// Map matrices
static int* topMap;
static int* bottomMap;

// Selector position (x, y)
static int selectorPos[2] = {0, 0};

// Tilemap sise
static int xSize = 12;
static int ySize = 12;

// Tilemap start position
static GLfloat tilemapX = 960.0f;
static GLfloat tilemapY = 120.0f;

// Textures
static unsigned int waterTexture;
static unsigned int emptyTexture;
static unsigned int groundTextures;
static unsigned int playerTextures;
static unsigned int goalTexture;

// Turn timer
static bool runningTurns = false;
static double turnTick = 0;

static bool loose = false;
static bool win   = false;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "IsoCross", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("shaders/transformations.vs", "shaders/transformations.frag");

	setupScene();

	resized = true;
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
        }
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
            keylock[key] = false;
        }
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

    if(win == true || loose == true){
         return;
    }

    if (keys[GLFW_KEY_W] && !keylock[GLFW_KEY_W]) {
        keylock[GLFW_KEY_W] = true;
        win = true;
        create_win_object("textures/win.png");
        return;
    }

    if (keys[GLFW_KEY_L] && !keylock[GLFW_KEY_L]) {
        keylock[GLFW_KEY_L] = true;
        loose = true;
        create_win_object("textures/loose.png");
        return;
    }

    if (keys[GLFW_KEY_UP] && !keylock[GLFW_KEY_UP]) {
        keylock[GLFW_KEY_UP] = true;
        if (selectorPos[0] != 0)
            selectorPos[0]--;
    }

    if (keys[GLFW_KEY_DOWN] && !keylock[GLFW_KEY_DOWN]) {
        keylock[GLFW_KEY_DOWN] = true;
        if (selectorPos[0] != ySize - 1)
            selectorPos[0]++;
    }

    if (keys[GLFW_KEY_LEFT] && !keylock[GLFW_KEY_LEFT]) {
        keylock[GLFW_KEY_LEFT] = true;
        if (selectorPos[1] != 0)
            selectorPos[1]--;
    }

    if (keys[GLFW_KEY_RIGHT] && !keylock[GLFW_KEY_RIGHT]) {
        keylock[GLFW_KEY_RIGHT] = true;
        if (selectorPos[1] != xSize - 1)
            selectorPos[1]++;
    }

    if (keys[GLFW_KEY_SPACE] && !keylock[GLFW_KEY_SPACE]) {
        keylock[GLFW_KEY_SPACE] = true;

        int pos = selectorPos[0] * xSize + selectorPos[1];
        if (!(topMap[pos] >= Tile::TileTexture::grassy_fence_back_right &&
             topMap[pos] <= Tile::TileTexture::fence_corner_left) &&
             bottomMap[pos] != Tile::TileTexture::water) {
            if (bottomMap[pos] == Tile::TileTexture::stone)
                bottomMap[pos] = Tile::TileTexture::grass;
            else bottomMap[pos] = Tile::TileTexture::stone;
        }
    }

    if (keys[GLFW_KEY_ENTER] && !keylock[GLFW_KEY_ENTER]) {
        keylock[GLFW_KEY_ENTER] = true;
        runningTurns = true;
    }

    // need to run next turn?
    if (runningTurns) {
        if (glfwGetTime() >= turnTick + 1.0) runTurn();
    }

}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

    if(win == true || loose == true ){
        objects[objects.size()-1]->update();
		objects[objects.size()-1]->draw();
        return;
    }

    // Update bottom tilemap
	for (int i = 0 ; i < (objects.size()-1) / 2 ; i++)
	{
        if (Tile::TileTexture(bottomMap[i]) != ((Tile *) objects[i])->texMap) {
            Tile *replacement = new Tile(Tile::TileTexture(bottomMap[i]));

            replacement->setPosition(objects[i]->getPosition());
            replacement->setDimension(objects[i]->getDimension());

            if (bottomMap[i] == Tile::TileTexture::water) {
                replacement->setTexture(waterTexture);
            } else if (bottomMap[i] == Tile::TileTexture::nothing) {
                replacement->setTexture(emptyTexture);
            }
            else {
                replacement->setTexture(groundTextures);
            }

            replacement->setShader(objects[i]->getShader());

            objects[i] = replacement;
        }

		objects[i]->update();
		objects[i]->draw();
	}
	
    // Update top tilemap
	for (int i = (xSize*ySize) + 1 ; i < objects.size() ; i++)
	{
        int mapPos = i - (xSize*ySize + 1);
        if (Tile::TileTexture(topMap[mapPos]) != ((Tile *) objects[i])->texMap) {
            Sprite *replacement;
            if (topMap[mapPos] >= Tile::TileTexture::enemy_idle && topMap[mapPos] != Tile::TileTexture::nothing) {
                replacement = new Character(Tile::TileTexture(topMap[mapPos]));
            } else {
                replacement = new Tile(Tile::TileTexture(topMap[mapPos]));
            } 

            replacement->setPosition(objects[i]->getPosition());
            replacement->setDimension(objects[i]->getDimension());

            if (topMap[mapPos] == Tile::TileTexture::nothing) {
                replacement->setTexture(emptyTexture);
            } else if (topMap[mapPos] == Tile::TileTexture::goal) {
                replacement->setTexture(goalTexture);
            } else if (topMap[mapPos] >= Tile::TileTexture::enemy_idle) {
                replacement->setTexture(playerTextures);
            }
            else {
                replacement->setTexture(groundTextures);
            }

            replacement->setShader(objects[i]->getShader());

            objects[i] = replacement;
        }

		objects[i]->update();
		objects[i]->draw();
	}
    
    // Update selector position
    GLfloat selectorX = tilemapX + (((xSize-1-selectorPos[0]) - (ySize-1-selectorPos[1])) * 128.0f/2);
    GLfloat selectorY = tilemapY + (((xSize-1-selectorPos[0]) + (ySize-1-selectorPos[1])) * 128.0f/4);
    objects[xSize*ySize]->setPosition(glm::vec3(selectorX, selectorY, 0.0));
    objects[xSize*ySize]->update();
    objects[xSize*ySize]->draw();
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
    // Load textures
    waterTexture = loadTexture("textures/water.png");
    emptyTexture = loadTexture("textures/Nothing.png");
    groundTextures = loadTexture("textures/basic_ground_tiles.png");
    playerTextures = loadTexture("textures/characters_reduced.png");
    goalTexture = loadTexture("textures/Goal.png");

    bottomMap = (int*) malloc(xSize * ySize * sizeof(int));
    loadMap("maps/dev_test.tilemap", xSize, ySize, bottomMap);
    makeTilemap(xSize, ySize, tilemapX, tilemapY, bottomMap);

    // Selector tile
    Tile* selector = new Tile(Tile::TileTexture::selector);

    GLfloat selectorX = tilemapX + (((xSize-1-selectorPos[0]) - (ySize-1-selectorPos[1])) * 128.0f/2);
    GLfloat selectorY = tilemapY + (((xSize-1-selectorPos[0]) + (ySize-1-selectorPos[1])) * 128.0f/4);

    unsigned int selectorTexture = loadTexture("textures/Selector.png");

    selector->setPosition(glm::vec3(selectorX, selectorY, 0.0));
    selector->setDimension(glm::vec3(128.0f, 128.0f, 1.0f));
    selector->setShader(shader);
    selector->setTexture(selectorTexture);
    objects.push_back(selector);

    // Top tilemap
    topMap = (int*) malloc(xSize * ySize * sizeof(int));
    loadMap("maps/dev_test_top.tilemap", xSize, ySize, topMap);
    makeTilemap(xSize, ySize, tilemapX, tilemapY + 60.0f, topMap);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 1920.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 1080.0f; //yMax

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::makeTilemap(int xSize, int ySize, GLfloat startX, GLfloat startY, int* map) {
    Sprite *til;

    for (int i = xSize - 1 ; i >= 0 ; i--) {
        for (int j = ySize - 1 ; j >= 0 ; j--) {
            int pos = (xSize - 1 - i) * xSize + (ySize - 1 - j);

            if (map[pos] >= Tile::TileTexture::enemy_idle)
                til = new Character(Tile::TileTexture(map[pos]));
            else
                til = new Tile(Tile::TileTexture(map[pos]));

            GLfloat pixelX = startX + ((i-j) * 128.0f/2);
            GLfloat pixelY = startY + ((i+j) * 128.0f/4);

            til->setPosition(glm::vec3(pixelX, pixelY, 0.0));
            til->setDimension(glm::vec3(128.0f, 128.0f, 1.0f)); 

            if (map[pos] == Tile::TileTexture::water) {
                til->setTexture(waterTexture);
            } else if (map[pos] == Tile::TileTexture::nothing) {
                til->setTexture(emptyTexture);
            } else if (map[pos] == Tile::TileTexture::goal) {
                til->setTexture(goalTexture);
            } else if (map[pos] >= Tile::TileTexture::enemy_idle) {
                til->setTexture(playerTextures);
            } else {
                til->setTexture(groundTextures);
            }

            til->setShader(shader);

            objects.push_back(til);
        }
     }
}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);


	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}

void SceneManager::loadMap(string filename, int mapSizeX, int mapSizeY, int* map) {
    std::ifstream mapFile (filename);

    int countRow = 0;
    string rawFile;

    if (mapFile.is_open()) {
        while (mapFile.good() && countRow < mapSizeY) {
            mapFile >> rawFile;

            for (int j = 0 ; j < mapSizeX ; j++) {
                map[countRow * mapSizeX + j] = std::stoi(rawFile.substr(j*2, 2));
            }
            countRow++;
        }
    }
}

void SceneManager::runTurn() {
    int pos = 0;
    Character::NextState state = Character::NextState::move;

    for (auto object : objects) {
        if (((Tile *)object)->texMap >= Tile::TileTexture::player_idle &&
            ((Tile *)object)->texMap != Tile::TileTexture::nothing &&
            ((Tile *)object)->texMap != Tile::TileTexture::goal)
            state = ((Character *)object)->followPath(bottomMap, topMap, pos-(xSize*ySize), xSize);

        else if (((Tile *)object)->texMap >= Tile::TileTexture::enemy_idle &&
                 ((Tile *)object)->texMap <= Tile::TileTexture::player_idle)
            ((Character *)object)->enemyPatrol(bottomMap, topMap, pos-(xSize*ySize), xSize, ySize);

        pos++;
    }

    if (state == Character::NextState::stop) runningTurns = false;
    turnTick = glfwGetTime();
}


void SceneManager::create_win_object(string path){
	Sprite *sprite = new Sprite;
	unsigned int texID = loadTexture(path);

	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(960.0f, 540.0f, 0.0));
	sprite->setDimension(glm::vec3(1920.0f, 1080.0f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

}
