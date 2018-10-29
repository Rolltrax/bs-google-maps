# bs-google-maps
Bindings to the standard JS Google Maps library, and a React component wrapper for bucklescript

## Info
🚧 WARNING: This is a WIP library. It is used in production here at Rolltrax, *but* if you don't find a feature you're looking for, it might be because we don't need it in house. If this is the case, feel free to open an issue (or, even better, a PR), as we will be monitoring this repo. 🚧

This library attempts to achieve 2 main goals:
- bind the Google Maps JS library to Bucklescript
- wrap the elements (map, autocomplete bar, etc.) in ReasonReact components

As of right now, the main component is `GoogleMap`, which renders the map in a div, which you can specify the height.

We plan to implement the autocomplete bar next (the one which suggests POIs).

## Maintainers
Zach Baylin - [me@zachbayl.in](mailto:me@zachbayl.in)

Ohad Rau - [admin@ohad.space](mailto:admin@ohad.space)

![rolltrax](https://rolltrax.com/img/favicon.ico) Rolltrax