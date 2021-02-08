
class Counter(dict):
    def __getitem__(self, idx):
        self.setdefault(idx, 0)
        return dict.__getitem__(self, idx)

    def __add__(self, x):
        result = Counter()
        for key in self:
            if key in x:
                result[key] = self[key] + x[key]
            else:
                result[key] = self[key]
        
        for key in x:
            if key in self:
                continue
            result[key] = x[key]
        
        return result

    def __sub__(self, x):
        result = Counter()
        for key in self:
            if key in x:
                result[key] = self[key] - x[key]
            else:
                result[key] = self[key]
        
        for key in x:
            if key in self:
                continue
            result[key] = x[key] * -1
        
        return result
            

    