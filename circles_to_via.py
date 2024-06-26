import pcbnew
board = pcbnew.GetBoard()


for line in board.GetDrawings():
    if "Окружность" in line.GetFriendlyName():
        newvia = pcbnew.PCB_VIA(board)
        # need to add before SetNet will work, so just doing it first
        board.Add(newvia)

        #newvia.SetPosition(board.GetDrawings()[0].GetEffectiveShape().Centre())
        newvia.SetDrill(int(1000000))
        newvia.SetWidth(int(2000000))
        newvia.SetLayerPair(board.GetLayerID('F.Cu'),board.GetLayerID('F.Cu'))
        newvia.SetViaType(pcbnew.VIATYPE_THROUGH)

        newvia.SetPosition(line.GetCenter())

        nets = board.GetNetsByName()
        newvia.SetNet(nets.begin().value()[1])
        board.Remove(line)

pcbnew.Refresh()