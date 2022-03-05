import vsync.*;
import processing.serial.*;

PFont font;

float pseudoValue1 = 0;
float pseudoValue2 = 0;
float timer = 0;

float fin1 = 0, fin2 = 0;

float altitude = 0;

float accel[] = {0};

ValueReceiver receiver;
ValueSender sender;
int isAbort = 0;
int packageSize = 0;
int packageNo = 0;
int snrValue = 0, rssiValue = 0;

void setup() {
	size(800, 800);
	// font = createFont("RobotoMono-Regular.ttf", 32);
	
	// This part communicates with the arduino
	Serial serial = new Serial(this, "COM3", 9600);
	// sender = new ValueSender(this, serial).observe("isAbort");
	receiver = new ValueReceiver(this, serial).observe("packageSize").observe("packageNo").observe("rssiValue").observe("snrValue");
}

void draw() {
	background(25);
	
	pseudoValues();
	
	// drawAltitude(60, 60, 200, 500);
	// drawFins(60, 60, 300);
	drawGraph(accel, 60, 60, 500, 300);
	// drawSequence(60, 60, 1500, 200);
	
	// textFont(font);
	textSize(16);
	text("packageNo: " + packageNo, 5, 30);
	text("packageSize: " + snrValue, 5, 60);
	text("rssiValue: " + rssiValue, 5, 90);
	text("snrValue: " + snrValue, 5, 120);
  text("isAbort: " + isAbort, 5, 150);
  
}

void keyPressed() {
  if (key == BACKSPACE) {
    giveAbort();
  }
}

void pseudoValues() { // Generate some fakeass values.
	timer++;
	pseudoValue1 = noise(timer * 0.03, 0);
	pseudoValue2 = noise(timer * 0.03, 100);
	
	fin1 = map(pseudoValue1, 0, 1, - 1, 1);
	fin2 = map(pseudoValue2, 0, 1, - 1, 1);
	
	accel = append(accel, map(pseudoValue1, 0, 1, - 1, 1));
	altitude += pseudoValue1 * 100;
}

void drawFins(int coorX, int coorY, int size) {
	PGraphics canvas;
	canvas = createGraphics(size, size);
	canvas.beginDraw();
	canvas.background(0);
	
	canvas.noFill();
	canvas.stroke(255);
	canvas.strokeWeight(2);
	canvas.ellipse(size / 2, size / 2, size / 3, size / 3);
	canvas.ellipse(size / 2, size / 2, size / 4, size / 4);
	canvas.ellipse(size / 2, size / 2, size / 5, size / 5);
	
	indvidualFin(canvas, size / 3, size / 3, size / 3, - HALF_PI - QUARTER_PI, fin1);
	indvidualFin(canvas, 2 * size / 3, 2 * size / 3, size / 3, QUARTER_PI, fin1);
	indvidualFin(canvas, 2 * size / 3, size / 3, size / 3, - QUARTER_PI, fin2);
	indvidualFin(canvas, size / 3, 2 * size / 3, size / 3, HALF_PI + QUARTER_PI, fin2);
	
	canvas.endDraw();
	image(canvas, coorX, coorY);
}

void indvidualFin(PGraphics canvas, int coorX, int coorY, int len, float angle, float attackAngle) {
	if (attackAngle < 0) {
	    canvas.fill(255, 0, 255, 120);
	} else {
	    canvas.noFill();
	}
	canvas.stroke(255, 0, 255);
	canvas.strokeWeight(2);
	
	canvas.beginShape();
	
	canvas.vertex(coorX + (len - attackAngle * len / 6) * cos(angle), coorY + (len + attackAngle * len / 6) * sin(angle));
	canvas.vertex(coorX + ( - attackAngle * len / 6) * cos(angle), coorY + (attackAngle * len / 6) * sin(angle));
	canvas.vertex(coorX + (attackAngle * len / 3) * cos(angle), coorY + ( - attackAngle * len / 3) * sin(angle));
	
	canvas.endShape(CLOSE);
}

void drawGraph(float data[], int coorX, int coorY, int sizeX, int sizeY) {
	PGraphics canvas;
	canvas = createGraphics(sizeX, sizeY);
	canvas.beginDraw();
	canvas.background(0);
	
	int edgeWidth = 30;
	int usableWidth = sizeX - 2 * edgeWidth;
	int usableHeight = sizeY - 2 * edgeWidth;
	
	canvas.translate(sizeX - edgeWidth, sizeY / 2);
	
	canvas.noFill();
	canvas.stroke(255);
	canvas.strokeWeight(2);
	canvas.beginShape();
	
	for (int i = max(data.length - usableWidth, 1); i < data.length; i++) {
	    canvas.vertex(i - data.length, map(data[i], - 1, 1, - usableHeight / 2, usableHeight / 2));
	}
	
	canvas.endShape();
	
	canvas.endDraw();
	image(canvas, coorX, coorY);
}

void drawSequence(int coorX, int coorY, int sizeX, int sizeY) {
	PGraphics canvas;
	canvas = createGraphics(sizeX, sizeY);
	canvas.beginDraw();
	canvas.background(0);
	
	canvas.endDraw();
	image(canvas, coorX, coorY);
}

void drawAltitude(int coorX, int coorY, int sizeX, int sizeY) {
	PGraphics canvas;
	canvas = createGraphics(sizeX, sizeY);
	canvas.beginDraw();
	canvas.background(0);
	
	int edgeWidth = 60;
	int lineCount = 30;
	int lineLength = (sizeY - 2 * edgeWidth) / (lineCount * 2);
	
	canvas.strokeWeight(2);
	canvas.stroke(255, 255, 255);
	for (int i = 0; i < lineCount + 1; i++) { // Draw vertical dashed line
	    canvas.line(sizeX / 2, edgeWidth + (2 * i) * lineLength, sizeX / 2, edgeWidth + (2 * i + 1) * lineLength);
	}
	canvas.line(0, sizeY - edgeWidth, sizeX, sizeY - edgeWidth); // Draw horizontal line
	
	canvas.noStroke();
	canvas.fill(255, 0, 255);
	
	canvas.rect(sizeX / 2 - 6, sizeY - edgeWidth - (altitude * (sizeY - 2 * edgeWidth) / 10000) - 20, 12, 20); // Draw the rocket
	
	canvas.textFont(font);
	canvas.textSize(16);
	
	canvas.text("altitude:" + nfc(int(altitude)), 10, sizeY - edgeWidth / 2);
	
	canvas.fill(255,255,255);
	canvas.text("10,000ft", sizeX / 2, edgeWidth / 2);
	
	canvas.endDraw();
	image(canvas, coorX, coorY);
}

void giveAbort() {
	isAbort = 1;
}

// void drawAB123(int coorX, int coorY, int sizeX, int sizeY) {
//     PGraphics canvas;
//     canvas = createGraphics(sizeX, sizeY);
//     canvas.beginDraw();
//     canvas.background(0);

//     canvas.endDraw();
//     image(canvas, coorX, coorY);
// }
