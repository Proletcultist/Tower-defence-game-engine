from conan import Workspace
from pathlib import Path

class TowerDefenceGameEngineWorkspace(Workspace):
    def name(self):
        return "Tower-defence-game-engine"

    def packages(self):
        return [
            {
                "path": Path("editor"),
                "ref": "tower-defence-game-editor/0.0.1",
            },
        ]
