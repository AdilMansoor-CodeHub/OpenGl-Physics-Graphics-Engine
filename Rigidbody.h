class Rigidbody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;

    Rigidbody(glm::vec3 initPos, float m) : position(initPos), velocity(glm::vec3(0.0f)), force(glm::vec3(0.0f)), mass(m) {}

    void applyForce(glm::vec3 f) {
        force += f;
    }

    void update(float deltaTime) {
        // Update velocity and position
        velocity += force * deltaTime / mass;
        position += velocity * deltaTime;

        // Reset force
        force = glm::vec3(0.0f);
    }
};
