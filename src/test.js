const { FastCSVParser, parse, parseToObjects } = require('./index');

// Test data
const csvData = `name,age,city
John,25,New York
Jane,30,"San Francisco"
Bob,35,Chicago
"Smith, Alice",28,Boston`;

console.log('Testing fast-csv-parser-native...\n');

// Test 1: Simple parse function
console.log('1. Simple parse function:');
const result1 = parse(csvData);
console.log(result1);
console.log();

// Test 2: Parse to objects
console.log('2. Parse to objects:');
const result2 = parseToObjects(csvData);
console.log(result2);
console.log();

// Test 3: Using class instance
console.log('3. Using class instance:');
const parser = new FastCSVParser({ delimiter: ',', hasHeader: true });
const result3 = parser.parseToObjects(csvData);
console.log('Headers:', parser.getHeaders());
console.log('Data:', result3);
console.log();

// Test 4: Custom delimiter
console.log('4. Custom delimiter (semicolon):');
const csvDataSemicolon = `name;age;city
John;25;New York
Jane;30;San Francisco`;

const result4 = parse(csvDataSemicolon, { delimiter: ';' });
console.log(result4);
console.log();

// Test 5: No header
console.log('5. No header:');
const csvNoHeader = `John,25,New York
Jane,30,San Francisco`;

const result5 = parse(csvNoHeader, { hasHeader: false });
console.log(result5);
console.log();

// Test 6: Performance test
console.log('6. Performance test:');
const largeCSV = 'name,age,city\n' + 
    Array.from({ length: 10000 }, (_, i) => `User${i},${20 + i % 50},City${i % 100}`).join('\n');

console.time('Native C++ Parse');
const largeResult = parse(largeCSV);
console.timeEnd('Native C++ Parse');
console.log(`Parsed ${largeResult.length} rows`);

console.log('\nAll tests completed successfully!');