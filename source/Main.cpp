#include "Scene.h"
#include "PinholeCamera.h"
#include "Renderer.h"

void Render() {
	Scene *scene = new Scene();
	EmissionMaterial *light = scene->CreateEmissionMaterial(Vector3(1, 1, 1) * 50);
	scene->CreateMesh(Pose(10, Vector3(0, -10, -10)), "./resource/plane.mesh", false, light)->Rotate(Vector3(1, 0, 0), -PI / 2);
	DiffuseMaterial *white = scene->CreateDiffuseMaterial(Vector3(1, 1, 1));
	scene->CreateMesh(Pose(4, Vector3(0, 2, 2)), "./resource/plane.mesh", false, white)->Rotate(Vector3(1, 0, 0), -PI / 2);
	scene->CreateMesh(Pose(4, Vector3(0, 0, 4)), "./resource/plane.mesh", false, white);
	GlossyMaterial *glossy = scene->CreateGlossyMaterial(Vector3(1, 1, 1), 20);
	scene->CreateBall(1, Vector3(0, 0, 2), glossy);
	// scene.CreateMesh(Pose(1, Vector3(0, 0, 2)), "./resource/ball.mesh", false, glossy)->Translate(Vector3(0.5, 0, 0));
	PinholeCamera *camera = new PinholeCamera(400, 400, Pose(), DegreeToRadian(90));
	// HostRender(RenderTarget(&scene, &camera, "host.png"), RenderOptions(10, 5));
	scene->BuildBVH();
	DeviceRender(RenderTarget(scene, camera, "device.png"), RenderOptions(10, 5));
}

int main() {

	// Test();
	Render();

	return 0;
}


// TODO
// Change MeshData::triangles_ from Vector<MeshTriangle> to Vector<MeshTriangle *>, and use new oprator.
