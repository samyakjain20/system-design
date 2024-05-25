#include <iostream>
#include <string>

// Base component - Text stream
class TextStream {
public:
  virtual void write(const std::string& text) = 0;
};

// Concrete component - Standard output stream
class StdOutStream : public TextStream {
public:
  void write(const std::string& text) override {
    std::cout << "--- StdOutStream ---\n";
    std::cout << text;
  }
};

// Decorator class - Adds encryption functionality
class EncryptingStream : public TextStream {
private:
  TextStream* stream_;
public:
  EncryptingStream(TextStream* stream) : stream_(stream) {}

  void write(const std::string& text) override {
    std::cout << "--- EncryptingStream ---\n";
    // Simulate simple encryption (Caesar Cipher)
    std::string encrypted;
    for (char c : text) {
      encrypted += (c + 3) % 128;
    }
    stream_->write(encrypted);
  }
};

// Decorator class - Adds uppercasing functionality
class UppercaseStream : public TextStream {
private:
  TextStream* stream_;
public:
  UppercaseStream(TextStream* stream) : stream_(stream) {}

  void write(const std::string& text) override {
    std::cout << "--- UppercaseStream ---\n";
    std::string uppercased;
    for (char c : text) {
      uppercased += toupper(c);
    }
    stream_->write(uppercased);
  }
};

int main() {
  TextStream* stream = new StdOutStream();

  // Write to standard output
  stream->write("Hello, world!\n");


  // Apply uppercase decorator on top of encryption
  stream = new UppercaseStream(stream);
  stream->write("More secrets! (uppercase)");

  
  // Apply encryption decorator
  stream = new EncryptingStream(stream);
  stream->write("This is a secret message.");
  std::cout << std::endl;

  delete stream; // Clean up memory
  return 0;
}
