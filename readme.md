# fast-csv-parser-native

A high-performance CSV parser for Node.js that uses N-API and C++ to offload parsing work from JavaScript to native code.

Written with assistance of Claude Sonnet 4 to explore how it well it works, so please use it carefully.

## Features

- **High Performance**: Native C++ implementation for maximum speed
- **Memory Efficient**: Minimal memory overhead compared to pure JS solutions
- **Flexible**: Support for custom delimiters, headers, and parsing options
- **Easy to Use**: Simple API with both functional and class-based interfaces
- **Robust**: Handles quoted fields, escaped characters, and edge cases
- **N-API**: Uses Node.js N-API for stability across Node.js versions

## Installation

```bash
npm install fast-csv-parser-native
```

**Prerequisites:**
- Node.js 16.0.0 or higher
- C++ compiler (Visual Studio on Windows, Xcode on macOS, GCC on Linux)
- Python (for node-gyp)

## Usage

### Quick Start

```javascript
const { parse, parseToObjects } = require('fast-csv-parser-native');

const csvData = `name,age,city
John,25,New York
Jane,30,San Francisco`;

// Parse as 2D array
const rows = parse(csvData);
console.log(rows);
// [['John', '25', 'New York'], ['Jane', '30', 'San Francisco']]

// Parse as objects
const objects = parseToObjects(csvData);
console.log(objects);
// [
//   { name: 'John', age: '25', city: 'New York' },
//   { name: 'Jane', age: '30', city: 'San Francisco' }
// ]
```

### Class-based API

```javascript
const { FastCSVParser } = require('fast-csv-parser-native');

const parser = new FastCSVParser({
  delimiter: ',',
  hasHeader: true,
  skipEmptyLines: true
});

const csvData = `name,age,city
John,25,New York
Jane,30,San Francisco`;

// Parse as objects
const objects = parser.parseToObjects(csvData);
console.log(objects);

// Get headers
const headers = parser.getHeaders();
console.log(headers); // ['name', 'age', 'city']

// Parse as 2D array
const rows = parser.parse(csvData);
console.log(rows);
```

### Options

All parsing functions accept an options object:

```javascript
const options = {
  delimiter: ',',        // Field delimiter (default: ',')
  hasHeader: true,       // Whether CSV has header row (default: true)
  skipEmptyLines: true   // Skip empty lines (default: true)
};

const result = parse(csvData, options);
```

### Advanced Usage

```javascript
// Custom delimiter
const csvData = `name;age;city
John;25;New York`;

const result = parse(csvData, { delimiter: ';' });

// No header row
const csvData = `John,25,New York
Jane,30,San Francisco`;

const result = parse(csvData, { hasHeader: false });

// Handle quoted fields with commas
const csvData = `name,description
John,"Lives in New York, NY"
Jane,"Works at Google, Inc"`;

const result = parseToObjects(csvData);
```

## API Reference

### Functions

#### `parse(csvData, options)`

Parses CSV data and returns a 2D array.

- `csvData` (string): The CSV data to parse
- `options` (object): Parser options
- Returns: `Array<Array<string>>` - 2D array of parsed data

#### `parseToObjects(csvData, options)`

Parses CSV data and returns an array of objects (requires headers).

- `csvData` (string): The CSV data to parse
- `options` (object): Parser options
- Returns: `Array<Object>` - Array of objects with header keys

### Class: FastCSVParser

#### `new FastCSVParser(options)`

Creates a new parser instance.

- `options` (object): Parser options

#### `parser.parse(csvData)`

Parses CSV data and returns a 2D array.

- `csvData` (string): The CSV data to parse
- Returns: `Array<Array<string>>` - 2D array of parsed data

#### `parser.parseToObjects(csvData)`

Parses CSV data and returns an array of objects.

- `csvData` (string): The CSV data to parse
- Returns: `Array<Object>` - Array of objects with header keys

#### `parser.getHeaders()`

Returns the headers from the last parsed CSV (if hasHeader was true).

- Returns: `Array<string>` - Header names

## Performance

This native implementation typically provides 2-5x performance improvement over pure JavaScript CSV parsers, especially for large files. The exact performance gain depends on:

- File size (larger files see better improvements)
- Number of fields per row
- Amount of quoted/escaped content
- System specifications

## Building from Source

```bash
# Clone the repository
git clone <repository-url>
cd fast-csv-parser-native

# Install dependencies
npm install

# Build the native module
npm run build

# Run tests
npm test
```

## Supported Platforms

- Windows (Visual Studio 2017+)
- macOS (Xcode)
- Linux (GCC)

## License

MIT

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.