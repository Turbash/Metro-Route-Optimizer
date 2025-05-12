#include "Visualization.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <QString>
#include <cmath>

using namespace std;

QString getLineColorHTML(const string &line)
{
    if (line == "Blue")
        return "#4169E1";
    if (line == "Yellow")
        return "#FFDF00";
    if (line == "Red")
        return "#FF4040";
    if (line == "Pink")
        return "#FC8EAC";
    if (line == "Magenta")
        return "#CC338B";
    if (line == "Violet")
        return "#8b5cf6";
    return "#333333";
}

QString getRouteHTML(const vector<int> &path, const vector<Station> &stations,
                     int travelTime, double totalDistance, int fare,
                     bool isHoliday, bool hasMetroCard)
{

    if (path.empty())
    {
        return "No valid route found.";
    }

    /* Calculate final fare with metro card discount if applicable */
    int finalFare = fare;
    bool hasDiscount = false;

    if (hasMetroCard)
    {
        /* Apply 10% discount for metro card */
        finalFare = static_cast<int>(ceil(fare * 0.9));
        hasDiscount = true;
    }

    int startId = path.front();
    int endId = path.back();

    /* Format the route details using HTML for better styling */
    QString routeInfoHTML = QString("<html><body style='font-family: Arial;'>");

    /* Header - Route title with larger font */
    routeInfoHTML += QString("<div style='margin-bottom: 8px;'>"
                             "<span style='font-size: 16px; font-weight: bold;'>Route from %1 to %2</span>"
                             "</div>")
                         .arg(QString::fromStdString(stations[startId].name))
                         .arg(QString::fromStdString(stations[endId].name));

    /* Journey details with text-based styling */
    routeInfoHTML += "<div style='margin: 10px 0; padding: 5px;'>";

    /* Time and distance info with larger text */
    routeInfoHTML += QString("<div style='margin-bottom: 5px;'>"
                             "<span style='font-weight: bold; color: #3b82f6;'>Time:</span> "
                             "<span style='font-size: 15px;'>%1 minutes</span>"
                             "</div>")
                         .arg(travelTime);

    routeInfoHTML += QString("<div style='margin-bottom: 5px;'>"
                             "<span style='font-weight: bold; color: #3b82f6;'>Distance:</span> "
                             "<span style='font-size: 15px;'>%1 KM</span>"
                             "</div>")
                         .arg(QString::number(totalDistance, 'f', 2));

    /* Fare info with prominent text styling and discount information */
    routeInfoHTML += QString("<div style='margin: 8px 0;'>"
                             "<span style='font-weight: bold; color: #10b981;'>Fare:</span> ");

    /* Show original fare and discounted fare if there's a discount */
    if (hasDiscount)
    {
        routeInfoHTML += QString("<span style='font-size: 15px; text-decoration: line-through; color: #666;'>₹%1</span> ")
                             .arg(fare);
        routeInfoHTML += QString("<span style='font-size: 18px; font-weight: bold; color: #10b981;'>₹%1</span>")
                             .arg(finalFare);
        routeInfoHTML += " <span style='font-size: 12px; color: #10b981;'>(10% card discount)</span>";
    }
    else
    {
        routeInfoHTML += QString("<span style='font-size: 18px; font-weight: bold; color: #10b981;'>₹%1</span>")
                             .arg(finalFare);
    }

    /* Add holiday information if applicable */
    if (isHoliday)
    {
        routeInfoHTML += " <span style='font-size: 12px; color: #10b981;'>(Holiday rate)</span>";
    }

    routeInfoHTML += "</div>";

    /* Path header with bold text and more visible color */
    routeInfoHTML += "<span style='font-size: 16px; font-weight: bold; color: #FF5500;'>Path:</span>";

    /* Prepare lines info for each station in the path */
    vector<vector<string>> path_lines;
    vector<string> station_names;
    vector<int> unique_path;

    /* Create a unique path without duplicate station names */
    for (int idx : path)
    {
        if (station_names.empty() || stations[idx].name != station_names.back())
        {
            station_names.push_back(stations[idx].name);
            unique_path.push_back(idx);
            path_lines.push_back(splitLines(stations[idx].line));
        }
    }

    string current_line = path_lines[0][0];
    /* Try to pick a line that continues to the next station */
    if (unique_path.size() > 1)
    {
        for (const string &l : path_lines[0])
        {
            if (find(path_lines[1].begin(), path_lines[1].end(), l) != path_lines[1].end())
            {
                current_line = l;
                break;
            }
        }
    }

    /* Start station with styled line text */
    routeInfoHTML += "<p style='margin: 8px 0;'><b>1. Start at</b> " +
                     QString::fromStdString(stations[unique_path[0]].name) + " [";

    for (size_t l = 0; l < path_lines[0].size(); ++l)
    {
        QString lineColor = getLineColorHTML(path_lines[0][l]);

        routeInfoHTML += QString("<span style='color: %1; font-weight: bold;'>%2</span>")
                             .arg(lineColor)
                             .arg(QString::fromStdString(path_lines[0][l]));

        if (l < path_lines[0].size() - 1)
            routeInfoHTML += "/";
    }
    routeInfoHTML += "]</p>";

    int step = 2;
    for (size_t i = 1; i < unique_path.size(); i++)
    {
        /* Check if current_line is present at this station */
        if (find(path_lines[i].begin(), path_lines[i].end(), current_line) == path_lines[i].end())
        {
            /* Need to change line */
            /* Find a new line that is present in both previous and current station */
            string new_line = path_lines[i][0];
            for (const string &l : path_lines[i])
            {
                if (find(path_lines[i - 1].begin(), path_lines[i - 1].end(), l) != path_lines[i - 1].end())
                {
                    new_line = l;
                    break;
                }
            }

            /* Line change notification with colored text */
            QString lineColor = getLineColorHTML(new_line);

            routeInfoHTML += QString("<p style='margin: 8px 0; padding: 5px;'>"
                                     "<b>%1. Change to</b> <span style='color: %2; font-weight: bold;'>%3 Line</span> at %4</p>")
                                 .arg(step++)
                                 .arg(lineColor)
                                 .arg(QString::fromStdString(new_line))
                                 .arg(QString::fromStdString(stations[unique_path[i - 1]].name));

            current_line = new_line;
        }

        /* Regular station with line text */
        routeInfoHTML += QString("<p style='margin: 8px 0;'><b>%1. →</b> %2 [")
                             .arg(step++)
                             .arg(QString::fromStdString(stations[unique_path[i]].name));

        for (size_t l = 0; l < path_lines[i].size(); ++l)
        {
            QString lineColor = getLineColorHTML(path_lines[i][l]);

            routeInfoHTML += QString("<span style='color: %1; font-weight: bold;'>%2</span>")
                                 .arg(lineColor)
                                 .arg(QString::fromStdString(path_lines[i][l]));

            if (l < path_lines[i].size() - 1)
                routeInfoHTML += "/";
        }
        routeInfoHTML += "]</p>";
    }

    routeInfoHTML += "</div></body></html>";
    return routeInfoHTML;
}
