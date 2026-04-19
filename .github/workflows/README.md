# CI/CD Workflows

## 🏁 ESP32 FreeRTOS Projects CI/CD

[![CI/CD Pipeline](https://github.com/farishu/esp32-freertos-projects/actions/workflows/ci.yml/badge.svg)](https://github.com/farishu/esp32-freertos-projects/actions/workflows/ci.yml)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-blue)](https://platformio.org)

This folder contains the GitHub Actions workflows for automated building and testing of all ESP32 FreeRTOS projects.

## Available Workflows

### ci.yml
Main CI/CD pipeline that:
- Builds all projects using PlatformIO
- Runs static analysis with cppcheck
- Uploads build artifacts (firmware.bin files)
- Checks code quality across all projects

## Usage

The workflow runs automatically on:
- Push to `main`, `master`, or `develop` branches
- Pull requests targeting these branches

To manually trigger a workflow run, go to the Actions tab in GitHub and select the workflow.
