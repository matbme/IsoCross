#include "Tile.h"
#include "Character.h"

Tile::Tile(TileTexture texMap) { initialize(texMap); }

void Tile::initialize(TileTexture texMap) {
    this->texMap = texMap;

    if (texMap == Tile::TileTexture::water) return Sprite::initialize();
    if (texMap == Tile::TileTexture::selector) return Sprite::initialize();

    int textureWidth, textureHeight, spriteRows;
    float texMapWidth, texMapHeight;

    if (texMap >= Tile::TileTexture::enemy_idle) {
        textureWidth = 45;
        textureHeight = 50;
        texMapWidth = 630.0;
        texMapHeight = 100.0;
        spriteRows = 14;
    } else {
        textureWidth = 128;
        textureHeight = 128;
        texMapWidth = 1024.0;
        texMapHeight = 896.0;
        spriteRows = 8;
    }

    float bot_left[2]   = {(texMap % spriteRows * textureWidth) / texMapWidth,
        static_cast<float> ((floor(texMap / spriteRows) * textureHeight) / texMapHeight)};

    float bot_right[2]  = {(texMap % spriteRows * textureWidth + textureWidth) / texMapWidth,
        static_cast<float> ((floor(texMap / spriteRows) * textureHeight) / texMapHeight)};

    float top_left[2]   = {(texMap % spriteRows * textureWidth) / texMapWidth,
        static_cast<float> ((floor(texMap / spriteRows) * textureHeight + textureHeight) / texMapHeight)};

    float top_right[2]  = {(texMap % spriteRows * textureWidth + textureWidth) / texMapWidth,
        static_cast<float> ((floor(texMap / spriteRows) * textureHeight + textureHeight) / texMapHeight)};

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
        top_right[0], top_right[1], // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        bot_right[0], bot_right[1], // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        bot_left[0],  bot_left[1], // bottom left
        -0.5f, 0.5f,  0.0f,  1.0f, 1.0f, 0.0f,
        top_left[0],  top_left[1] // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    transform = glm::mat4(1); // matriz identidade
    texID = -1;               // ainda não temos
    shader = NULL;            // ainda não temos
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    angle = 0.0f;
}
