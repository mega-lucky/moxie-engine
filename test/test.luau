--!nocheck
--!optimize 2

const Transform = World.GetComponentID("Transform")
const MeshShape = World.GetComponentID("MeshShape")
const MeshRender = World.GetComponentID("MeshRender")
const Camera = World.GetComponentID("Camera")

do
    const entity = World.NewEntity()
    World.GiveComponent(entity, Transform)
    World.GiveComponent(entity, MeshShape)
    World.GiveComponent(entity, MeshRender)

    World.GetComponent(entity, MeshShape).Mesh = "Cube"
    World.GetComponent(entity, MeshRender).Material = "Checkers"

    const transform = World.GetComponent(entity, Transform)
    transform.Position = vector.create(0, -2, 0)
    transform.Scale = vector.create(10, 1, 10)
end

const entity = World.NewEntity()
World.GiveComponent(entity, Transform)
World.GiveComponent(entity, MeshShape)
World.GiveComponent(entity, MeshRender)

World.GetComponent(entity, MeshShape).Mesh = "Cube"
World.GetComponent(entity, MeshRender).Material = "Checkers"

const camera_entity = World.NewEntity()
World.GiveComponent(camera_entity, Transform)
World.GiveComponent(camera_entity, Camera)

const cam_data = World.GetComponent(camera_entity, Camera)
cam_data.Projection = "Perspective"
cam_data.YFov = math.rad(45)
cam_data.AspectRatio = 8/6
cam_data.Near = 0.01
cam_data.Far = 200

const ct = World.GetComponent(camera_entity, Transform)
ct.Scale = vector.one

const transform = World.GetComponent(entity, Transform)
transform.Position = vector.create(0, 0, 0)
transform.Scale = vector.create(1, 1, 1)

local velo = vector.zero
const accel = 40
const drag = 5
local camrotvel = vector.zero
local camrot = vector.zero

while true do
    const dt = time.wait()

    local movement = vector.zero
    if Input.IsKeyDown(Input.KeyInput.W) then
        movement += vector.create(0, 0, -1)
    end
    if Input.IsKeyDown(Input.KeyInput.A) then
        movement += vector.create(-1, 0, 0)
    end
    if Input.IsKeyDown(Input.KeyInput.S) then
        movement += vector.create(0, 0, 1)
    end
    if Input.IsKeyDown(Input.KeyInput.D) then
        movement += vector.create(1, 0, 0)
    end

    camrotvel = vector.zero
    if Input.IsKeyDown(Input.KeyInput.Left) then
        camrotvel += vector.create(0, 1, 0) * math.tau * 10
    end
    if Input.IsKeyDown(Input.KeyInput.Right) then
        camrotvel += vector.create(0, 1, 0) * math.tau * -10
    end
    if Input.IsKeyDown(Input.KeyInput.Up) then
        camrotvel += vector.create(1, 0, 0) * math.tau * -10
    end
    if Input.IsKeyDown(Input.KeyInput.Down) then
        camrotvel += vector.create(1, 0, 0) * math.tau * 10
    end
    camrot += camrotvel * dt

    const yrot = Quaternion.New(0, 1, 0, math.rad(camrot.y))
    const rotation = yrot * Quaternion.New(1, 0, 0, math.rad(camrot.x))
    const acceleration = (yrot * movement) * accel

    velo += acceleration * dt
    transform.Position += velo * dt
    velo -= velo * drag * dt
    
    transform.Rotation = yrot
    ct.Position = rotation * vector.create(0.5, 0, 8) + transform.Position
    ct.Rotation = rotation
end