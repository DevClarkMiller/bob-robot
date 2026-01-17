import React from 'react';
import { Card, CardContent } from './ui/card';
import { Input } from './ui/input';
import { Label } from './ui/label';

export interface UnitGuidInputProps {
	unitGuid: string;
	setUnitGuid: React.Dispatch<React.SetStateAction<string>>;
}

const UnitGuidInput = ({ unitGuid, setUnitGuid }: UnitGuidInputProps) => {
	return (
		<Card>
			<CardContent>
				<Label className="mb-2">Unit GUID</Label>
				<Input
					placeholder="87487969-0636-4196-9e1a-718f6c80e626"
					value={unitGuid}
					onChange={e => setUnitGuid(e.target.value)}
				/>
			</CardContent>
		</Card>
	);
};

export default UnitGuidInput;
