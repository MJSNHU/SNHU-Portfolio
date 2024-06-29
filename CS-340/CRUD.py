from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object):
    """CRUD operators for Animal collection in MogoDB"""
    
    def __init__(self, USER='user', PASS='pass'):
        # Initializing the Mongo Client. This helps to
        # access the MongoDB databases and collections.
        # This is hardwired to use the aac database, the
        # animals collection, and the aac user.
        # Definitions of the ocnnection string variables are
        # unique to the individual Apporto environment.
        #
        # You must edit the connection variables below to reflect
        # your own instance of MongoDB!
        #
        # Connection Variables
        #
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 31853
        DB = 'AAC'
        COL = 'animals'
        #    
        # Initialize Connection
        #
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (USER, PASS, HOST, PORT))
        self.database = self.client['%s' % (DB)]
        self.collection = self.database['%s' % (COL)]
               
    # Complete this create method to implement the C in CRUD.
    def create(self, data):
        if data is not None:
            try:
                if self.read({'animal_id':data['animal_id']}):   #Check for duplicate animal ID
                    print("Create Error: duplicate animal_id")
                    return False
                else:
                    self.database.animals.insert_one(data)  # data should be in dictionary format
                    return True         # sucessful insertion
            except Exception as e:  # if there is any exception, return False
                print("Create Error:", e)
                return False                
        else:
            raise Exception("Nothing to save, because data parameter is empty")
        
    # Create method to implement the R in CRUD
    def read(self, data):
        if data is not None:
            try:
                result = list(self.collection.find(data))
                return result
            except Exception as e:
                print("Read Error:", e)
                return []
        else:
            raise Exception("Nothing to read, because data parameter is empty")
            
    # Create method to implement U in CRUD
    def update(self, fltr, update, one=True):        # default create one record only
        if fltr is not None and update is not None:                    
            try:
                if not one:
                    return self.collection.update_many(fltr, update).modified_count
                else:
                    return self.collection.update_one(fltr, update).modified_count
            except Exception as e:
                print ("Update Error:", e)
                return 0
        else:
            raise Exception("Nothing to update, because filter or update parameter is empty")
    
    # Create method to impement D in CRUD
    def delete(self, data, one=True):                 # default delete one record only
        if data is not None:
            try:
                if not one:
                    return self.collection.delete_many(data).deleted_count
                else:
                    return self.collection.delete_one(data).deleted_count
            except Exception as e:
                print ("Delete Error:", e)
                return 0
        else:
            raise Exception("Nothing to delete, because data parameter is empty")
            
    