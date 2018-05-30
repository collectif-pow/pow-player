import { combineReducers } from 'redux';
import players from './players';
import scan from './scan';
import scanned from './scanned';

const rootReducer = combineReducers({
	players,
	scan,
	scanned,
});

export default rootReducer;
