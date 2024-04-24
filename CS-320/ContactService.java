package contact;

import java.util.ArrayList;

public class ContactService {

	// create array of Contacts
	private ArrayList<Contact> contacts = new ArrayList<Contact>();
	
	// check for duplicate ID, if so throw exception
	private boolean duplicateID(String ID) {
		for (Contact c: contacts) {
		    if (c.getID().equals(ID)) {
		    	throw new IllegalArgumentException("Duplicate ID");
		    	//return true;
	 	    }
		}
		return false;
	}
	
	// add Contact with only an ID;
	// ensures ID is not duplicated
	// Contact constructor with only ID fills remaining 
	// fields with "empty"
    public void addContact(String ID) {
    	Contact c = new Contact(ID);
    	if (!duplicateID(ID)) {
     	    contacts.add(c);
    	}
    }
	
    // add Contact with full fields
    // ensures ID is not duplicated
    public void addContact(String ID, String firstName, String lastName, String phone, String address) {
    	
    	Contact c = new Contact(ID, firstName, lastName, phone, address);
    	if (!duplicateID(c.getID())) {
    	    contacts.add(c);
    	}
    	
    }
    
    // add contact given full contact
    // ensures ID is not duplicated
    public void addContact(Contact c) {
    	if (!duplicateID(c.getID())) {
    		contacts.add(c);
    	}
    }
   
    // search through contact list
	// remove contact if it matches the given ID
    public void deleteContact(String ID) {
        for (int i = 0; i < contacts.size(); i++) {
        	if(contacts.get(i).getID().equals(ID)) {
        	    contacts.remove(i); 
        	}
        }
    }
    
    // find contact with given ID, update first name
    public void updateFirstName(String ID, String firstName) {
        	for (Contact c: contacts) {
    		if (c.getID().equals(ID)) {
       			c.setFirstName(firstName);
       		}
    	}
    }
    	
        
    // find contact with given ID, update last name
    public void updateLastName(String ID, String lastName){
    	for (Contact c: contacts) {
    		if (c.getID().equals(ID)) {
    			c.setLastName(lastName);
    		}
    	}
    }
 
    // find contact with given ID, update phone number
    public void updateNumber(String ID, String number) {
    	for (Contact c: contacts) {
    		if (c.getID().equals(ID)) {
    			c.setNumber(number);
    		}
    	}
    }
    
    // find contact with given ID, update address
    public void updateAddress(String ID, String address) {
    	for (Contact c: contacts) {
    		if (c.getID().equals(ID)) {
    			c.setAddress(address);
    		}
    	};
    	
    }
    
    public Contact getContact(String ID) {
    	for (Contact c: contacts) {
    		if (c.getID().equals(ID)) {
    			return c;
    		}
    	}
    	return null;
    }
    
}
