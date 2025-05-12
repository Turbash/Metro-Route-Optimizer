# Metro Route Optimizer

## Overview
Metro Route Optimizer is a Qt-based desktop application that helps users find optimal routes in the Delhi Metro network. The application calculates the shortest path between stations, estimates travel time, distance, and fare while considering factors like holidays/weekends and Metro Card discounts.

## Features
- Interactive metro map visualization
- Station selection from alphabetical lists
- One-click station swapping
- Shortest path calculation using Dijkstra's algorithm
- Fare estimation based on distance and day type
- Metro Card discount calculation
- Multi-line route visualization
- Highlight of the optimal path on the map

## How to Run

### Prerequisites
- Qt 5.12 or later
- C++11 compatible compiler

### Build from Source
1. Clone the repository:
   ```
   git clone https://github.com/yourusername/metro-route-optimizer.git 
   cd metro-route-optimizer
   ```

2. Open `MetroProject.pro` in Qt Creator

3. Build and run the project from Qt Creator, or use qmake:
   ```
   qmake
   make
   ./MetroRoute
   ```

### Deployment
To deploy the application:

1. Build in release mode:
   ```
   qmake CONFIG+=release
   make
   ```

2. Use the appropriate deployment tool for your platform:
   - Windows: Use `windeployqt`:
     ```
     windeployqt --release MetroRoute.exe
     ```
   - macOS: Use `macdeployqt`:
     ```
     macdeployqt MetroRoute.app -dmg
     ```
   - Linux: Package according to your distribution requirements

## Future Improvements

### Network Data
- Add actual Delhi Metro distances and travel times
- Include more stations for comprehensive coverage
- Add newer metro lines (Grey, Orange, etc.)
- Incorporate real-time data for service disruptions

### User Interface
- Improve map rendering with better scaling and zooming
- Add dark mode option
- Implement responsive design for different screen sizes
- Add accessibility features

### Features
- Incorporate interchange times between lines
- Add multiple route options (fastest vs. fewest transfers)
- Support for offline usage
- Save favorite routes
- Show nearby landmarks/attractions at stations
- Multi-language support

## Contributing
Contributions to improve the Metro Route Optimizer are welcome! If you'd like to contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Areas where help is especially appreciated:
- UI/UX improvements
- Accurate metro network data
- Performance optimizations
- Additional features

## License
This project is licensed under the MIT License - see below for details:

```
MIT License

Copyright (c) 2025 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Acknowledgments
- Delhi Metro Rail Corporation for inspiring this project
- Qt framework developers
- All contributors and users