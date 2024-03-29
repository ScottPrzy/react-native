/**
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @flow strict-local
 * @format
 * @oncall react_native
 */

import net from 'net';
import fetch from 'node-fetch';

/**
 * Determine whether we can run the dev server.
 *
 * Return values:
 * - `not_running`: The port is unoccupied.
 * - `matched_server_running`: The port is occupied by another instance of this
 *   dev server (matching the passed `projectRoot`).
 * - `port_taken`: The port is occupied by another process.
 * - `unknown`: An error was encountered; attempt server creation anyway.
 */
export default async function isDevServerRunning(
  host: string,
  port: number,
  projectRoot: string,
): Promise<
  'not_running' | 'matched_server_running' | 'port_taken' | 'unknown',
> {
  try {
    if (!(await isPortOccupied(host, port))) {
      return 'not_running';
    }

    const statusResponse = await fetch(`http://localhost:${port}/status`);
    const body = await statusResponse.text();

    return body === 'packager-status:running' &&
      statusResponse.headers.get('X-React-Native-Project-Root') === projectRoot
      ? 'matched_server_running'
      : 'port_taken';
  } catch (e) {
    return 'unknown';
  }
}

async function isPortOccupied(host: string, port: number): Promise<boolean> {
  let result = false;
  const server = net.createServer();

  return new Promise((resolve, reject) => {
    server.once('error', e => {
      server.close();
      if (e.code === 'EADDRINUSE') {
        result = true;
      } else {
        reject(e);
      }
    });
    server.once('listening', () => {
      result = false;
      server.close();
    });
    server.once('close', () => {
      resolve(result);
    });
    server.listen({host, port});
  });
}
