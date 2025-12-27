import pandas as pd
a = [1,2,3,4]
b = ["sIUI", "IUIdfg", "IUazdI", "IUaze"]
dico = {"a" : a, "b" : b}
df = pd.DataFrame(dico)
print(df)