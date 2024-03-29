#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <M5CoreS3.h>
#include <SD.h>

extern const char* API_KEY;
const char* openai_endpoint = "https://api.openai.com/v1";

String completions(const String& content) {
  HTTPClient http;
  http.begin(String(openai_endpoint) + "/chat/completions");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(API_KEY));
  http.setTimeout(60000);

  String postData = "{\"model\": \"gpt-4\", \"messages\": [{\"role\": \"user\", \"content\": \"" + content + "\"}]}";

  int httpResponseCode = http.POST(postData);
  String text = "";

  String response = http.getString();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);

  if (httpResponseCode > 0) {
    JsonArray choices = doc["choices"];
    for (JsonObject choice : choices) {
      text = choice["message"]["content"].as<String>();
    }
  } else {
    JsonObject error = doc["error"];
    text = error["code"].as<String>();
  }

  http.end();

  return text;
}

void textToSpeech(const String& content) {
  HTTPClient http;
  http.begin(String(openai_endpoint) + "/audio/speech");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(API_KEY));
  http.setTimeout(60000);

  String postData = "{\"model\": \"tts-1\", \"voice\": \"alloy\", \"input\": \"" + content + "\"}";
  int httpResponseCode = http.POST(postData);

  String response = http.getString();
  printEfont("post: ");
  printEfont(const_cast<char*>(String(httpResponseCode).c_str()));
  printEfont(const_cast<char*>(response.c_str()));
  printEfont(const_cast<char*>(content.c_str()));

  if (httpResponseCode > 0) {
    File file = SD.open("/speech.mp3", FILE_WRITE);
    if (file) {
      http.writeToStream(&file);
      file.close();
    } else {
      printEfont("Failed to open file on SD card");
    }
  } else {
    printEfont("TextToSpeech でエラー発生");
  }

  http.end();
}