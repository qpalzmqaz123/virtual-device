'use strict'

const assert = require('assert')

class Transport {
  constructor (sock) {
    this._sock = sock
    this._buffer = Buffer.alloc(0)
  }

  _read () {
    return new Promise((resolve, reject) => {
      this._sock.once('data', chunk => resolve(chunk))
    })
  }

  _write (data) {
    return new Promise((resolve, reject) => {
      this._sock.write(data, resolve)
    })
  }

  _analyzeHeader () {
    if (this._buffer.length < 12) {
      return null
    }

    return {
      model: this._buffer.readUInt32LE(0),
      id: this._buffer.readUInt32LE(4),
      length: this._buffer.readUInt32LE(8)
    }
  }

  _getFrame () {
    const header = this._analyzeHeader()
    if (!header || this._buffer.length < 12 + header.length) {
      return null
    }

    const data = this._buffer.slice(12, 12 + header.length)
    this._buffer = this._buffer.slice(12 + header.length)

    return Object.assign(header, { data })
  }

  _genHeader (frame) {
    const header = Buffer.alloc(12)

    header.writeUInt32LE(frame.model, 0)
    header.writeUInt32LE(frame.id, 4)
    header.writeUInt32LE(frame.length, 8)

    return header
  }

	/**
	 * Frame object
	 * @typedef {Object} Frame
	 * @property {number} model
	 * @property {number} id
	 * @property {number} length
	 * @property {Buffer} data
	 */

  /**
   * Read frame
   *
   * @returns {Frame}
   */
  async read () {
    while (true) {
      let data = await this._read()
      this._buffer = Buffer.concat([ this._buffer, data ])

      const frame = this._getFrame()
      if (frame) {
        return frame
      }
    }
  }

  /**
   * Write frame
   *
   * @param {Frame} frame
   */
  async write (frame) {
    assert(Buffer.isBuffer(frame.data))

    const header = this._genHeader(frame)
    const data = Buffer.concat([ header, frame.data ])

    this._write(data)
  }
}

exports.Transport = Transport
