#!/usr/bin/env node
'use strict'

const program = require('commander')
const defaultConfig = require('../config/default.json')
const version = require('../package.json').version
const backend = require('../lib')

program
  .version(version)
  .option('-s, --sock <path>', `socket path, default is '${defaultConfig.socketPath}'`)
  .parse(process.argv)

const options = {
  socketPath: program.sock || defaultConfig.socketPath
}

backend.run(options)
