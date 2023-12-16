#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 50, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 40 < 1) {

		this->random_seed = ofRandom(1000);
	}

	ofSeedRandom(this->random_seed);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	string word = "01234567";

	auto number_of_sphere = 350;
	vector<glm::vec3> location_list;
	vector<float> radius_list;

	while (location_list.size() < number_of_sphere) {

		auto location = glm::vec3(ofRandom(-250, 250), ofRandom(-250, 250), ofRandom(-250, 250));
		auto radius = ofRandom(10, 70);
		bool flag = true;
		for (int i = 0; i < location_list.size(); i++) {

			if (glm::distance(location, location_list[i]) < (radius + radius_list[i])) {

				flag = false;
				break;
			}
		}

		if (flag) {

			location_list.push_back(location);
			radius_list.push_back(radius);
		}
	}

	int count = 0;
	while (true) {

		auto all = true;

		for (int i = 0; i < location_list.size(); i++) {

			auto flag = true;
			for (int k = 0; k < location_list.size(); k++) {

				if (i == k) { continue; }

				if (glm::distance(location_list[i], location_list[k]) < (radius_list[i] + radius_list[k])) {

					flag = false;
				}
			}

			if (flag) {

				radius_list[i] += 1;
				all = false;
				break;
			}
		}

		if (all) {

			break;
		}

		count++;
	}

	ofColor color;
	auto param = ofGetFrameNum() % 40 < 5 ? 0.2 : ofGetFrameNum() % 40 < 15 ? ofMap(ofGetFrameNum() % 40, 5, 15, 0.2, 1) : 1;
	for (int i = 0; i < location_list.size(); i++) {

		color.setHsb(ofRandom(255), 150, 255);

		int word_index = radius_list[i] > 30 ? word.size() - 1 : ofMap(radius_list[i], 8, 30, 0, word.size() - 1);

		ofPath chara_path = this->font.getCharacterAsPoints(word[word_index], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();

		auto angle = glm::vec3(ofRandom(-PI * 0.5, PI * 0.5), ofRandom(-PI * 0.5, PI * 0.5), ofRandom(-PI * 0.5, PI * 0.5));
		auto rotation_x = glm::rotate(glm::mat4(), angle.x, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), angle.y, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), angle.z, glm::vec3(0, 0, 1));

		ofSetColor(color);
		ofFill();
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			outline[outline_index] = outline[outline_index].getResampledByCount(150);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();

			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				glm::vec3 vertex = location_list[i] + glm::vec4(((vertices[vertices_index] - glm::vec2(25, -25)) / 50) * radius_list[i] * 1.5 * param, 0) * rotation_z * rotation_y * rotation_x;
				ofVertex(vertex);
			}
		}
		ofEndShape(true);

		ofSetColor(0);
		ofNoFill();
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			outline[outline_index] = outline[outline_index].getResampledByCount(150);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();

			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				glm::vec3 vertex = location_list[i] + glm::vec4(((vertices[vertices_index] - glm::vec2(25, -25)) / 50) * radius_list[i] * 1.5 * param, 0) * rotation_z * rotation_y * rotation_x;
				ofVertex(vertex);
			}
		}
		ofEndShape(true);
	}

	this->cam.end();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}