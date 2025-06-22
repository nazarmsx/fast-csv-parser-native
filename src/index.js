const addon = require('../build/Release/csv_parser');

/**
 * High-performance CSV parser using native C++ implementation
 */
class FastCSVParser {
    /**
     * Create a new CSV parser instance
     * @param {Object} options - Parser options
     * @param {string} options.delimiter - Field delimiter (default: ',')
     * @param {boolean} options.hasHeader - Whether CSV has header row (default: true)
     * @param {boolean} options.skipEmptyLines - Skip empty lines (default: true)
     */
    constructor(options = {}) {
        this.parser = new addon.CSVParser(options);
    }

    /**
     * Parse CSV data and return as 2D array
     * @param {string} csvData - CSV data to parse
     * @returns {Array<Array<string>>} Parsed data as 2D array
     */
    parse(csvData) {
        return this.parser.parse(csvData);
    }

    /**
     * Parse CSV data and return as array of objects (requires headers)
     * @param {string} csvData - CSV data to parse
     * @returns {Array<Object>} Parsed data as array of objects
     */
    parseToObjects(csvData) {
        return this.parser.parseToObjects(csvData);
    }

    /**
     * Get the headers from the CSV (if hasHeader was true)
     * @returns {Array<string>} Header names
     */
    getHeaders() {
        return this.parser.getHeaders();
    }
}

/**
 * Quick parse function for simple use cases
 * @param {string} csvData - CSV data to parse
 * @param {Object} options - Parser options
 * @returns {Array<Array<string>>} Parsed data as 2D array
 */
function parse(csvData, options = {}) {
    return addon.parse(csvData, options);
}

/**
 * Parse CSV and return as objects with header keys
 * @param {string} csvData - CSV data to parse
 * @param {Object} options - Parser options
 * @returns {Array<Object>} Parsed data as array of objects
 */
function parseToObjects(csvData, options = {}) {
    const parser = new FastCSVParser(options);
    return parser.parseToObjects(csvData);
}

module.exports = {
    FastCSVParser,
    parse,
    parseToObjects
};